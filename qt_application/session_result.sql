select DISTINCT player_kit.ses_id, player_kit.plr_name,
	(select count(*) from hit_view where hit_view.hunter_name = player_kit.plr_name and hunter_name != pray_name and hit_view.ses_id = player_kit.ses_id) as hit_cnt,
	(select count(*) from hit_view where hit_view.hunter_name = player_kit.plr_name and hunter_name != pray_name AND hit_view.hit_is_death=1 and hit_view.ses_id = player_kit.ses_id) as kill_cnt,
	ifnull((select sum(hit_view.hit_damage) from hit_view where hit_view.hunter_name = player_kit.plr_name and hunter_name != pray_name and hit_view.ses_id = player_kit.ses_id), 0) as sum_damage,
	
	(select count(*) from hit_view where hit_view.pray_name = player_kit.plr_name and hit_view.ses_id = player_kit.ses_id) as wound_cnt,
	(select count(*) from hit_view where hit_view.pray_name = player_kit.plr_name AND hit_view.hit_is_death=1 and hit_view.ses_id = player_kit.ses_id) as death_wound_cnt,
	(select sum(hit_view.hit_damage) from hit_view where hit_view.pray_name = player_kit.plr_name and hunter_name != pray_name and hit_view.ses_id = player_kit.ses_id) as absorded_damage_sum
from player_kit inner join hit_detector on player_kit.det_id=hit_detector.det_id