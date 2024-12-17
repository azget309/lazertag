select DISTINCT h_kit.ses_id, h_kit.plr_name as hunter_name, p_kit.plr_name as pray_name,
    (
        select count(*) from hit_view
        where 
			hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name and
			ses_id == p_kit.ses_id
    ) as hit_cnt,
	ifnull((
        select sum(hit_damage) from hit_view
        where 
            hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name and
			ses_id == p_kit.ses_id
    ),0) as damage_sum,
	(
        select count(*) from hit_view
        where 
            hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name and
			hit_is_death == 1 and
			ses_id == p_kit.ses_id
    ) as death_hit_cnt
from player_kit as h_kit, player_kit as p_kit