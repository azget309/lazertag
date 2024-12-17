select ses.ses_id, gst.gst_gun_model as gun_model,
	(
		select count(*) from hit_view
		where 
			hit_is_death and
			gst_id = gst.gst_id and
			ses_id=ses.ses_id AND
			pray_kit_id != hunter_kit_id
	) as kill_cnt
from play_session as ses, gun_settings as gst
where kill_cnt > 0