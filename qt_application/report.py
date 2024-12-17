import os

from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtCore import QUrl
from PySide6.QtGui import QPageLayout, QPageSize
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtSql import *

import plotly 
from plotly.offline import iplot
plotly.offline.init_notebook_mode(connected=True)
import plotly.io as pio
import plotly.graph_objects as go 
from plotly.subplots import make_subplots

pio.templates.default = 'plotly_dark'

layout = go.Layout(
    paper_bgcolor='rgba(0,0,0,0.5)',
    plot_bgcolor='rgba(0,0,0,0)',
)

def CreateKillTable(db, ses_id):
    sql = ''
    with open("hunter_pray_table.sql", 'r') as file:
        sql = file.read()

    query = QSqlQuery(sql+f" where h_kit.ses_id = {ses_id}")
    players = set()
    data = {}
    max_kill = 1
    max_damage = 1
    max_hit = 1
    while query.next():
        hunter_name = query.value(query.record().indexOf('hunter_name'))
        pray_name = query.value(query.record().indexOf('pray_name'))
        damage_sum = query.value(query.record().indexOf('damage_sum'))
        hit_cnt = query.value(query.record().indexOf('hit_cnt'))
        death_hit_cnt = query.value(query.record().indexOf('death_hit_cnt'))

        players.add(hunter_name)

        data[hunter_name, pray_name] = (death_hit_cnt, damage_sum, hit_cnt)

        if(max_hit < hit_cnt) : max_hit = hit_cnt
        if(max_damage < damage_sum) : max_damage = damage_sum
        if(max_kill < death_hit_cnt) : max_kill = death_hit_cnt

    players = list(players)
    
    stat_table = [
        [
        f'kills: <b>{data[p,h][0]}</b><br>damage: <b>{data[p,h][1]}</b><br>hits: <b>{data[p,h][2]}</b>'
            for p in players
        ] for h in players
    ]
    
    color_table = [
        [
            f'rgba({200*data[p,h][0]/max_kill},0,0,0.8)'
            for p in players
        ] for h in players
    ]

    fig = go.Figure(data=[go.Table(
        header=dict(
            values=[""]+players,
            fill=dict(color=['rgba(0,0,0,0)','rgba(0,0,0,0.5)'])
        ),
        cells=dict(
            values=[players]+stat_table,
            fill=dict(color=['rgba(0,0,0,0.5)']+color_table)
        ),
    )])

    fig.layout=layout
    fig.layout.title=dict(text="Таблица попаданий", font=dict(size=30), automargin=True, yref='paper')
    return fig



def CreateKillDethPie(db, ses_id):
    i_plr_names = []
    i_plr_kills = []
    i_plr_deaths= []
    i_plr_damage= []


    plr_names_kills = []
    plr_names_death = []
    plr_kills = []
    plr_deaths= []
    plr_damage= []
    r_colors = []; b_colors = []
    
    def AAAAAAAAAAA():
        i_plr_names.clear()
        i_plr_kills.clear()
        i_plr_deaths.clear()
        i_plr_damage.clear()


        plr_names_kills.clear()
        plr_names_death.clear()
        plr_kills.clear()
        plr_deaths.clear()
        plr_damage.clear()
        r_colors.clear(); b_colors.clear()

        while query.next():
            i_plr_names.append(query.value(query.record().indexOf('plr_name')))
            i_plr_kills.append(query.value(query.record().indexOf('kill_cnt')))
            i_plr_deaths.append(query.value(query.record().indexOf('death_wound_cnt')))
            i_plr_damage.append(query.value(query.record().indexOf('sum_damage')))
        
        max_to_enother = 128
        pie_plr_limit=min(max_to_enother, len(i_plr_names))
        for i in range(pie_plr_limit):
            cl = (230/pie_plr_limit)*(i+1)
            r_colors.append(f'rgb(255,{cl},{cl})')
            b_colors.append(f'rgb({cl},{cl},255)')
            plr_names_kills.append(i_plr_names[i])
            plr_names_death.append(i_plr_names[i])
            plr_kills.append(i_plr_kills[i])
            plr_deaths.append(i_plr_deaths[i])
            plr_damage.append(i_plr_damage[i])

        if(max_to_enother < len(i_plr_names)):
            plr_names_kills.append('прочие килы')
            plr_names_death.append('прочие килы')
        plr_kills.append(sum(i_plr_kills)-sum(plr_kills))
        plr_deaths.append(sum(i_plr_deaths)-sum(plr_deaths))
        plr_damage.append(sum(i_plr_damage)-sum(plr_damage))


    sql = ''
    with open("session_result.sql", 'r') as file:
        sql = file.read()


    fig = make_subplots(rows=1, cols=2, specs=[[{'type':'domain'},{'type':'domain'}]])

    query = QSqlQuery(sql + f" where player_kit.ses_id={ses_id} order by kill_cnt desc")
    AAAAAAAAAAA()

    fig.add_trace(
            go.Pie(
                labels=plr_names_kills,
                values=plr_kills,
                hole=0.4,
                textinfo='label+value',
                #textfont_size = 20,
                marker_colors=r_colors
            ),
            1,1
        )


    query = QSqlQuery(sql + f" where player_kit.ses_id={ses_id} order by death_wound_cnt desc")
    AAAAAAAAAAA()

    fig.add_trace(
            go.Pie(
                labels=plr_names_death,
                values=plr_deaths,
                hole=0.4,
                textinfo='label+value',
                #textfont_size = 20,
                marker_colors=b_colors
            ),
            1,2
        )
    
    fig.layout = layout

    fig.update_layout(
        title=dict(text="Лидеры по килам и смертям", font=dict(size=30), automargin=True, yref='paper'),
        annotations=[
                dict(
                    text='KILLS',
                    x=sum(fig.get_subplot(1,1).x)/2,
                    y=sum(fig.get_subplot(1,1).y)/2,
                    font_size=20, showarrow=False, xanchor="center"
                ),
                
                dict(
                    text='DEATHS',
                    x=sum(fig.get_subplot(1,2).x)/2,
                    y=sum(fig.get_subplot(1,2).y)/2,
                    font_size=20, showarrow=False, xanchor="center"
                )
            ]
        )

    return fig


def CreateEndTable(db, ses_id):
    sql = ''
    with open("session_result.sql", 'r') as file:
        sql = file.read()
    query = QSqlQuery(sql+f" where player_kit.ses_id={ses_id} order by kill_cnt desc")

    plr_name = []
    kill_cnt = []
    death_wound_cnt = []
    absorded_damage_sum = []
    sum_damage = []

    while query.next():
        plr_name.append(query.value(query.record().indexOf('plr_name')))
        kill_cnt.append(query.value(query.record().indexOf('kill_cnt')))
        death_wound_cnt.append(query.value(query.record().indexOf('death_wound_cnt')))
        sum_damage.append(query.value(query.record().indexOf('sum_damage')))
        absorded_damage_sum.append(query.value(query.record().indexOf('absorded_damage_sum')))
    
    stat_table = [plr_name] + [kill_cnt] + [death_wound_cnt] + [sum_damage] + [absorded_damage_sum]

    fig = go.Figure(data=[go.Table(
        header=dict(
            values=["игрок", "килы", "смерти", "нанесенный урон", "впитанный урон"],
            fill=dict(color=['rgba(0,0,0,0.3)']),
            font=dict(size=20),
        ),
        cells=dict(
            values=[]+stat_table,
            fill=dict(color=['rgba(0,0,0,0.3)','rgba(0,0,0,0.7)']),
            font=dict(size=20),
            height=40,
        ),
    )])

    fig.layout=layout

    return fig


def CreateFavoritGunGistagram(db, ses_id):
    sql = ''
    with open("favorit_gun.sql", 'r') as file:
        sql = file.read()

    query = QSqlQuery(sql+f" and ses_id = {ses_id} order by kill_cnt;")
    
    gun_model = []
    kill_cnt = []
    while query.next():
        gun_model.append(query.value(query.record().indexOf('kill_cnt')))
        kill_cnt.append(query.value(query.record().indexOf('gun_model')))
    fig = go.Figure(go.Bar(x=gun_model, y=kill_cnt, orientation='h'))
    fig.layout=layout
    fig.layout.xaxis=dict(title="Количество килов", tickformat=',d', tickfont=dict(size=20))
    fig.layout.yaxis=dict(title="Оружее", tickfont = dict(size=25))
    fig.layout.title=dict(text="Лучшее оружее", automargin=True, yref='paper')
    fig.layout.font=dict(size=30)

    return fig


def CreateFavoritGunPie(db, ses_id):
    sql = ''
    with open("hunter_pray_table.sql", 'r') as file:
        sql = file.read()
    query = QSqlQuery(f"\
        where h_kit.ses_id = {ses_id};\
    ")



def GenerateHtmlReport(db, ses_id):
    html = f"<script src='file://{os.path.dirname(os.path.realpath(__file__))}/plotly.js'></script>"
    html += '<html><body style="min-width: 1500px; min-height: 900px">'
    html += '<div style="margin: 30px 30px 30px 30px;">'+ plotly.offline.plot(
            CreateKillDethPie(db,ses_id),
            include_plotlyjs=False,
            output_type='div',
            auto_open=False)+'</div>'
    html += '<div style="margin: 30px 30px 30px 30px;">'+ plotly.offline.plot(
            CreateKillTable(db,ses_id),
            include_plotlyjs=False,
            output_type='div',
            auto_open=False
            )+'</div>'
    html += '<div style="margin: 30px 30px 30px 30px;">'+ plotly.offline.plot(
            CreateEndTable(db,ses_id),
            include_plotlyjs=False,
            output_type='div',
            auto_open=False) +'</div>'
    html += '<div style="margin: 30px 30px 30px 30px;">'+ plotly.offline.plot(
            CreateFavoritGunGistagram(db,ses_id),
            include_plotlyjs=False,
            output_type='div',
            auto_open=False) +'</div>'
    html += '</body></html>'

    return html



class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.db = QSqlDatabase.addDatabase('QSQLITE')
        self.db.setDatabaseName('./lazertag_db.sqlite')
        self.db.open()
    
        
        html = GenerateHtmlReport(self.db, 1)

        plot_widget = QWebEngineView()
        plot_widget.setHtml(
            html,
            baseUrl=QUrl.fromLocalFile(os.path.dirname(os.path.realpath(__file__)))
        )
        # set the QWebEngineView instance as main widget
        def tmp(finished):
            layout = QPageLayout()
            layout.setPageSize(QPageSize(QPageSize.A4Extra))
            layout.setOrientation(QPageLayout.Landscape)
            plot_widget.show()
            plot_widget.page().printToPdf("test.pdf", layout)

        plot_widget.loadFinished.connect(tmp)
        #self.setCentralWidget(plot_widget)

if __name__ == '__main__':
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()

