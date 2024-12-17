import sqlite3
import random as rn
import datetime



class player:
    def __init__(self,l,g):
        self.live = l
        self.resp = 0
        self.gun = g


db = sqlite3.connect('lazertag_db.sqlite')
cursor = db.cursor()


start = datetime.datetime(2010,10,10, 1,0,0)
end = datetime.datetime(2010,10,10, 2,0,0)

guns = {
    "dg":25,
    "ak":25,
    "m4":20,
    "awp":100
}

players = [
    player(100, 'awp'),
    player(100, 'ak'),
    player(100, 'ak'),
    player(100, 'dg'),
    player(100, 'm4'),
    player(100, 'm4'),
    player(100, 'awp'),
    player(100, 'awp'),
    player(100, 'dg'),
    player(100, 'dg'),
    player(100, 'ak'),
    player(100, 'ak')
]




req = []

while end > start:
    start += datetime.timedelta(seconds=rn.randint(1,10))
    for i in range(0,len(players)):
        if players[i].resp == 1:
            players[i].live = 100
            players[i].resp = 0
        if players[i].resp>0:
            players[i].resp -= 1 

    hu = rn.randint(0,len(players)-1)
    pr = rn.randint(0,len(players)-1)

    while hu==pr or players[pr].live==0 or players[hu].live==0:
        hu = rn.randint(0,len(players)-1)
        pr = rn.randint(0,len(players)-1)


    players[pr].live -= guns[players[hu].gun]

    if players[pr].live <= 0:
        players[pr].live = 0
        players[pr].resp = rn.randint(1, 4)
    
    kaka = (
            start.strftime("%Y-%m-%d %H:%M:%S"),
            pr+1, hu+1,
            1 if players[pr].live==0 else 0,
            guns[players[hu].gun]
        )
    print(kaka)
    req.append( kaka )

    #print(start.strftime("%Y-%m-%d %H:%M:%S"))
    #print(f"INSERT INTO hit (hit_time, det_id, gun_id, hit_is_death, hit_damage)")

for r in req:
    cursor.execute("INSERT INTO hit (hit_time, det_id, gun_id, hit_is_death, hit_damage) VALUES (?, ?, ?, ?, ?)", r)
    print(f"INSERT INTO hit (hit_time, det_id, gun_id, hit_is_death, hit_damage) VALUES (?, ?, ?, ?, ?)", r)

db.commit()
db.close()

