
-- ========================== таблица попаданий игроков по игрокам + ИД конфигурации ==================
DROP VIEW "main"."hit_view";
CREATE VIEW hit_view AS
select 
	hit.*,
	hunter_kit.ses_id as ses_id,
	hunter_kit.plr_name as hunter_name,
	(
        select kit.kit_id from kit 
        where 
			kit.gun_id = hit.gun_id 
			datetime(kit.kit_change_time) = (
            select max(datetime(kit.kit_change_time))
            from kit
            where
				kit.gun_id = hit.gun_id and
                datetime(kit.kit_change_time) <= datetime(hit.hit_time)
                
        )
    )as hunter_kit_id,
	pray_kit.plr_name as pray_name,
	(
        select kit.kit_id from kit
        where 
			kit.det_id = hit.det_id and
			datetime(kit.kit_change_time) = (
            select max(datetime(kit.kit_change_time))
            from kit
            where
				kit.det_id = hit.det_id and
                datetime(kit.kit_change_time) <= datetime(hit.hit_time)
                
        )
    )as pray_kit_id,
	(
		select gst.gst_id
		from gun_settings_on_session as gss
			inner join gun_settings as gst on gss.gst_id == gst.gst_id
		where gss.gun_id = hit.gun_id
	) as gst_id
from hit, player_kit as hunter_kit, player_kit as pray_kit
where hunter_kit_id = hunter_kit.kit_id and pray_kit_id = pray_kit.kit_id

-- ========================== таблица комплекта игрока на сеанс ==================
DROP VIEW IF EXISTS "main"."player_kit";
CREATE VIEW player_kit AS
select kit.*, player.plr_name, play_session.ses_id from kit, play_session, player
where
	player.ses_id = play_session.ses_id AND player.det_id = kit.det_id AND
	datetime(kit.kit_change_time) BETWEEN datetime(play_session.ses_start_date)
		and datetime(play_session.ses_start_date, play_session.ses_session_time)


-- ======================== запросы ==========================
-- вывести таблицу кто кого сколько раз убил (перекрестный запрос)
-- результат: сессия, убийца, жертва, кол-во ранений, кол-во смертельных ранений, сумма урона
select DISTINCT h_kit.ses_id, h_kit.plr_name, p_kit.plr_name,
    (
        select count(*) from hit_view
        where 
			hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name
    ) as hit_cnt,
	(
        select sum(hit_damage) from hit_view
        where 
            hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name
    ) as damage_sum,
	(
        select count(*) from hit_view
        where 
            hunter_name == h_kit.plr_name  and
            pray_name == p_kit.plr_name and
			hit_is_death == 1
    ) as death_hit_cnt
from kit as h_kit, kit as p_kit


-- вывести статистику игрока
-- результат: номер сессии; игрок; кол-во килов; кол-во смертей, нанесенный урон, впитаный урон
select DISTINCT kit.ses_id, kit.plr_name, 
	(select count(*) from hit_view where hit_view.hunter_name = kit.plr_name) as hit_cnt,
	(select count(*) from hit_view where hit_view.hunter_name = kit.plr_name AND hit_view.hit_is_death=1) as kill_cnt,
	(select sum(hit_view.hit_damage) from hit_view where hit_view.hunter_name = kit.plr_name) as sum_damage,
	
	(select count(*) from hit_view where hit_view.pray_name = kit.plr_name) as wound_cnt,
	(select count(*) from hit_view where hit_view.pray_name = kit.plr_name AND hit_view.hit_is_death=1) as death_wound_cnt,
	(select sum(hit_view.hit_damage) from hit_view where hit_view.pray_name = kit.plr_name) as absorded_damage_sum
from kit


-- вывести любимое оружее игрока
-- результат: номер сессии; игрок; номер оружия; кол-во килов из оружия
select distinct cur_kit.ses_id, cur_kit.plr_name, cur_kit.gun_id,
	(
		select count(*) from hit_view where
			hunter_name = cur_kit.plr_name and
			hit_is_death=1 and 
			hunter_name != pray_name
	) as kill_cnt
from kit as cur_kit
order by plr_name


-- вывести самое убойное оружее по игроку сесии
-- результат: сессия; оружее; кол-во килов из оружия; общее время использования оружия
select DISTINCT kit.ses_id, kit.gun_id, 
	(
		select count(*) from hit_view as hv
		where kit.gun_id=hv.gun_id and hv.hit_is_death=1 and hunter_name!=pray_name
	) as kill_cnt
from kit
order by kill_cnt desc;


--* динамика игры на периуд сеанса для указанного игрока(динамика смерти/килы для каждого игрока)


-- ======================= добавление данных =========================
-- добавить новый сеанс
--      вход: дата/время начала, продолжительность
-- изменить время сеанса
--      вход: id сеанса, новая дата начал, новая продолжительность
-- добавить игрока к сеансу
--      вход: имя игрока, номер датчика попаданий
-- обновить датчик игрока, привязанному к сеансу
-- обновить оружее игрока, привязанному к сеансу
