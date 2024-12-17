select DISTINCT plr_kit.ses_id, plr_kit.plr_name, gst.gst_gun_model as gun_model,
	(
		select count(*)
		from hit_view as hv
		where
			hv.hunter_name = plr_kit.plr_name and
			hv.hit_is_death=1 and
			hv.hunter_kit_id != hv.pray_kit_id and
			hv.gst_id = gst.gst_id and
			hv.ses_id = plr_kit.ses_id
	) as kill_cnt
from player_kit as plr_kit, gun_settings as gst
where kill_cnt > 0