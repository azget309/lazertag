WITH new_kit(det_id, gun_id, change_time) AS (SELECT 1, 2, datetime('now'))

select * from kit
where 
	kit.det_id = (select det_id from new_kit) and
	datetime(kit.kit_change_time) = (
	select max(datetime(kit.kit_change_time))
	from kit
	where
		kit.det_id = (select det_id from new_kit) and
		datetime(kit.kit_change_time) <= (select change_time from new_kit)
)