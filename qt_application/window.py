import sys
import os
import threading
import time
import requests
import re

# from widgets
from PySide6 import QtCore, QtGui, QtWidgets
from PySide6.QtUiTools import loadUiType
from PySide6.QtGui import QRegularExpressionValidator
from PySide6.QtCore import QRegularExpression

# from database
from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtCore import QUrl
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtSql import *

# from report
#import plotly 
#from plotly.offline import iplot
#plotly.offline.init_notebook_mode(connected=True)
#import plotly.io as pio
#import plotly.graph_objects as go 
#from plotly.subplots import make_subplots

from select_gun_config import AddGun

db_name = 'lazertag_db.sqlite'

generated_class, base_class = loadUiType("ui/main_window.ui")

gst_gun_reload_types = {
        0:('по нажаютию кнопки', 'скорость перезарядки', True),
        1:('помповая дозарядка', 'скорость дозорядки', True),
        2:('сменой магазина', 'скорость перезарядки', True),
    }

gst_gun_burst_types = {
        0:'ручное досылание патрона (скользящий затвор)',
        1:'одиночные выстрелы',
        2:'с отсечкой по 2 патрноа',
        3:'с отсечкой по 3 патрноа',
        4:'с отсечкой по 4 патрноа',
        5:'с отсечкой по 5 патрноа',
        6:'с отсечкой по 6 патрноа',
        255:'автоматический огонь'
}
gst_gun_burst_type_to_num = {v:k for k,v in gst_gun_burst_types.items()}

gst_gun_damage_types = {
    0:'1',
    1:'2',
    2:'4',
    3:'5',
    4:'7',
    5:'10',
    6:'15',
    7:'17',
    8:'20',
    9:'25',
    10:'30',
    11:'35',
    12:'40',
    13:'50',
    14:'75',
    15:'100'
}
gst_gun_damage_type_to_num = {v:k for k,v in gst_gun_damage_types.items()}

value_to_fire_temp = {
    0 : 250,
    1 : 300,
    2 : 350,
    3 : 400,
    4 : 450,
    5 : 500,
    6 : 550,
    7 : 600,
    8 : 650,
    9 : 700,
    10: 750,
    11: 800,
}

value_to_stun_time = {
    0:'0',
    1:'0.25',
    2:'0.5',
    3:'0.75',
    4:'1',
    5:'2',
    6:'3',
    7:'4',
    8:'5',
    9:'6',
    10:'7',
    11:'8',
    12:'9',
    13:'10',
    14:'11',
    15:'12',
    16:'13',
    17:'14',
    18:'15',
    19:'16',
    20:'17',
    21:'18',
    22:'19',
    23:'20'
}

value_to_health = {
    1 :1 , 2 :2 , 3 :3 , 4 :4 , 5 :5 , 6 :6 , 7 :7 ,
    8 :8 , 9 :9 , 10:10, 11:11, 12:12, 13:13, 14:14,
    15:15, 16:16, 17:17, 18:18, 19:19, 20:20,
    21:25, 22:30, 23:35, 24:40, 25:45, 26:50, 27:55,
    28:60, 29:65, 30:70, 31:75, 32:80, 33:85, 34:90,
    35:95, 36:100, 37:105, 38:110, 39:115, 40:120, 41:125,
    42:130, 43:135, 44:140, 45:145, 46:150, 47:155, 48:160,
    49:165, 50:170, 51:175, 52:180, 53:185, 54:190, 55:195, 56:200, 
    57:250, 58:300, 59:350, 60:400, 61:450, 62:500,
    63:550, 64:600, 65:650, 66:700, 67:750,
    68:800, 69:850, 70:900, 71:950, 72:999
}

class Validator(QtGui.QValidator):
    def __init__(self, pat):
        super().__init__()
        self.pattern = re.compile(pat)
    def validate(self, string, index):
        if string == "":
            return QtGui.QValidator.State.Acceptable, string, index

        if self.pattern.fullmatch(string):
            return QtGui.QValidator.State.Acceptable, string, index
        else:
            return QtGui.QValidator.State.Invalid, string, index

class MyApp(base_class, generated_class):
    selected_gst_id = -1
    def __init__(self):
        super().__init__()
        generated_class.__init__(self)
        self.setupUi(self)

        self.db = QSqlDatabase.addDatabase('QSQLITE')
        self.db.setDatabaseName(db_name)
        self.db.open()
        self.db.exec("PRAGMA foreign_keys=ON") # чтоб каскадное обновление работало

        self.tabWidget.currentChanged.connect(self.tab_changed_event)

        self.configurator_init()
        self.loader_init()

    def tab_changed_event(self, index: int):
        if(index == 0):
            self.update_config_list()
        elif(index == 1):
            self.update_inventary_gun_table()

    # ====================== CONFIGURATOR FUNCTION =========================#
        
        #self.gst_gun_name_line_edit.setValidator(v)

    def configurator_init(self):
        self.gst_gun_name_line_edit.setValidator(Validator("[0-9a-zA-Z _]+"))
        self.gst_gun_model_line_edit.setValidator(Validator("[0-9a-zA-Z _]+"))
        self.remove_gun_config_button.clicked.connect(
                self.remove_gun_config_button_click)
        self.add_gun_config_button.clicked.connect(
                self.add_gun_config_button_click)
        self.gun_config_list_widget.itemSelectionChanged.connect(
                self.select_gun_config)
        self.update_config_list()

        self.gst_gun_damage_combo_box.clear()
        for k,v in gst_gun_damage_types.items():
            self.gst_gun_damage_combo_box.addItem(str(v))
        
        self.gst_gun_reload_type_combo_box.clear()
        for k,v in gst_gun_reload_types.items():
            self.gst_gun_reload_type_combo_box.addItem(v[0])
        
        self.gst_gun_burst_size_combo_box.clear()
        for k,v in gst_gun_burst_types.items():
            self.gst_gun_burst_size_combo_box.addItem(v)

        self.gst_gun_name_line_edit.editingFinished.connect(
                self.update_gst_gun_name)
    
        self.gst_gun_model_line_edit.editingFinished.connect(
            lambda:self.update_cur_gst_value('gst_gun_model',
                        self.gst_gun_model_line_edit.text()))
        
        self.gst_player_live_count_spin_box.editingFinished.connect(
                self.update_gst_player_live_count)

        self.gst_stun_time_slider.valueChanged.connect(
                self.update_gst_player_stun_time)

        self.gst_friendlyfire_check_box.stateChanged.connect(
            lambda:self.update_cur_gst_value('gst_friendlyfire',
                1 if self.gst_friendlyfire_check_box.isChecked() else 0))

        self.gst_gun_damage_combo_box.currentIndexChanged.connect(
                self.update_gst_gun_damage)

        self.gst_gun_fire_temp_slider.valueChanged.connect(
                self.update_gst_gun_fire_temp)

        self.gst_gun_reload_type_combo_box.currentIndexChanged.connect(
                self.update_gst_gun_reload_type)

        self.gst_gun_burst_size_combo_box.currentIndexChanged.connect(
                self.update_gst_gun_burst_size)
        
        self.gst_gun_reload_time_spin_box.editingFinished.connect(
            lambda:self.update_cur_gst_value('gst_gun_reload_time',
                        self.gst_gun_reload_time_spin_box.value()))

        
        self.gst_gun_clip_size_spin_box.editingFinished.connect(
            lambda:self.update_cur_gst_value('gst_gun_clip_size',
                        self.gst_gun_clip_size_spin_box.value()))
        
        self.gst_gun_clip_count_spin_box.editingFinished.connect(
            lambda:self.update_cur_gst_value('gst_gun_clip_count',
                        self.gst_gun_clip_count_spin_box.value()))

    def update_cur_gst_value(self, val_name, val_value):
        QSqlQuery(f"\
            UPDATE gun_settings\
            SET {val_name} = \"{val_value}\"\
            WHERE gst_id = \"{self.selected_gst_id}\"").exec()

    def update_gst_gun_name(self):
        self.update_cur_gst_value('gst_gun_name', self.gst_gun_name_line_edit.text())
        self.update_config_list()
    
    def update_gst_player_live_count(self):
        val = self.gst_player_live_count_spin_box.value()
        key = min(value_to_health, key=lambda x: abs(value_to_health[x]-val))
        min_val = value_to_health[key]
        self.gst_player_live_count_spin_box.setValue(min_val)
        self.update_cur_gst_value('gst_player_live_count', str(key))

    def update_gst_player_stun_time(self, val):
        self.update_cur_gst_value('gst_player_stan_time', val)
        self.gst_stun_time_label.setText(
            'время шока '+value_to_stun_time[val]+' сек')

    def update_gst_gun_fire_temp(self, val):
        self.update_cur_gst_value('gst_gun_fire_temp', val)
        self.gst_gun_fire_temp_label.setText(
                'темп огня '+str(value_to_fire_temp[val])+' выст/мин')

    def update_gst_gun_damage(self, index):
        if(index == -1): return;
        self.update_cur_gst_value(
                'gst_gun_damage',
                gst_gun_damage_type_to_num[self.gst_gun_damage_combo_box.currentText()])

    def update_gst_gun_reload_type(self, index):
        if(index == -1): return;
        self.update_cur_gst_value( 'gst_gun_reload_type',index)
        self.reload_time_lable.setText(gst_gun_reload_types[index][1])
    
    def update_gst_gun_burst_size(self, index):
        if(index == -1): return;
        burst_type = self.gst_gun_burst_size_combo_box.itemText(index)
        self.update_cur_gst_value(
                'gst_gun_burst_size',
                gst_gun_burst_type_to_num[burst_type])

    def add_gun_config_button_click(self):
        sql = "INSERT INTO gun_settings (gst_gun_name) VALUES ('NewConfig');"
        query = QSqlQuery(sql)
        self.update_config_list()

    def remove_gun_config_button_click(self):
        flag_gst_in_gst_on_st = True
        
        if(flag_gst_in_gst_on_st):
            self.update_cur_gst_value('gst_gun_name', '[УДАЛЕНА]'+self.gst_gun_name_line_edit.text())
            self.update_cur_gst_value('gst_is_deleted', 1)
        else:
            QSqlQuery(f'DELETE FROM gun_settings WHERE gst_id="{self.selected_gst_id}"')

        self.update_config_list()


    def select_gun_config(self):
        sl_conf = self.gun_config_list_widget.currentItem().data(QtCore.Qt.UserRole)
        self.selected_gst_id = sl_conf
        self.player_param_group_box.setEnabled(True)
        self.gun_name_group_box.setEnabled(True)
        self.gun_param_group_box.setEnabled(True)
        self.update_gun_config(sl_conf)

    def update_config_list(self):
        self.gun_config_list_widget.blockSignals(True)
        self.gun_config_list_widget.clear()
        sql = 'select * from gun_settings where gst_is_deleted=0'
        query = QSqlQuery(sql)
        old_item = False
        while query.next():
            gst_gun_name = query.value(query.record().indexOf('gst_gun_name'))
            gst_id = query.value(query.record().indexOf('gst_id'))
            item = QListWidgetItem(gst_gun_name)
            item.setData(QtCore.Qt.UserRole, gst_id)
            self.gun_config_list_widget.addItem(item)
            if(gst_id == self.selected_gst_id):
                old_item = item
        self.gun_config_list_widget.blockSignals(False)

        if not old_item:
            self.selected_gst_id = -1
            self.player_param_group_box.setEnabled(False)
            self.gun_name_group_box.setEnabled(False)
            self.gun_param_group_box.setEnabled(False)
        else:
            self.gun_config_list_widget.setCurrentItem(old_item)

    def update_gun_config(self, gst_id):
        # blocked input event signal
        self.gst_gun_damage_combo_box.blockSignals(True)
        self.gst_gun_burst_size_combo_box.blockSignals(True)
        self.gst_gun_reload_type_combo_box.blockSignals(True)
        sql = f'select * from gun_settings where gst_id="{gst_id}"'
        query = QSqlQuery(sql)
        while query.next():
            gst_gun_name = query.value(query.record().indexOf('gst_gun_name'))
            gst_gun_reload_type = query.value(query.record().indexOf('gst_gun_reload_type'))
            gst_gun_fire_temp = query.value(query.record().indexOf('gst_gun_fire_temp'))
            gst_gun_burst_size = query.value(query.record().indexOf('gst_gun_burst_size'))
            gst_gun_damage = query.value(query.record().indexOf('gst_gun_damage'))
            gst_gun_clip_size = query.value(query.record().indexOf('gst_gun_clip_size'))
            gst_gun_clip_count = query.value(query.record().indexOf('gst_gun_clip_count'))
            gst_player_live_count = query.value(query.record().indexOf('gst_player_live_count'))
            gst_player_stan_time = query.value(query.record().indexOf('gst_player_stan_time'))
            gst_friendlyfire = query.value(query.record().indexOf('gst_friendlyfire'))
            gst_gun_model = query.value(query.record().indexOf('gst_gun_model'))
            gst_gun_reload_time = query.value(query.record().indexOf('gst_gun_reload_time'))

            
            self.gst_gun_name_line_edit.setText(gst_gun_name)
            self.gst_gun_model_line_edit.setText(gst_gun_model)

            self.gst_gun_damage_combo_box.setCurrentText(str(gst_gun_damage_types[gst_gun_damage]))
            self.gst_gun_fire_temp_slider.setValue(gst_gun_fire_temp)
            self.gst_gun_clip_size_spin_box.setValue(gst_gun_clip_size)
            self.gst_gun_clip_count_spin_box.setValue(gst_gun_clip_count)
            self.gst_gun_burst_size_combo_box.setCurrentText(
                gst_gun_burst_types[gst_gun_burst_size]
                    if gst_gun_burst_size in gst_gun_burst_types.keys() else
                gst_gun_burst_types[255]
                    )
            self.gst_gun_reload_time_spin_box.setValue(gst_gun_reload_time)
            self.gst_gun_reload_type_combo_box.setCurrentText(
                    gst_gun_reload_types[gst_gun_reload_type][0])
            self.reload_time_lable.setText(
                    gst_gun_reload_types[gst_gun_reload_type][1])
            self.gst_gun_reload_time_spin_box.setEnabled(
                    gst_gun_reload_types[gst_gun_reload_type][2])

            self.gst_player_live_count_spin_box.setValue(value_to_health[gst_player_live_count])
            self.gst_stun_time_slider.setValue(gst_player_stan_time)
            self.gst_stun_time_label.setText(
                'время шока '+value_to_stun_time[gst_player_stan_time]+' сек')
            self.gst_friendlyfire_check_box.setChecked(
                   True if gst_friendlyfire==1 else False)


        self.gst_gun_damage_combo_box.blockSignals(False)
        self.gst_gun_burst_size_combo_box.blockSignals(False)
        self.gst_gun_reload_type_combo_box.blockSignals(False)

    # ====================== LOADER FUNCTION =========================#
    def loader_init(self):
        self.update_inventary_gun_table()
        self.load_config_on_gun_button.clicked.connect(self.load_config_on_gun_clicked)
    
    def update_inventary_gun_table(self):
        model = QSqlQueryModel()
        model.setQuery("""
        SELECT gun.gun_id, gst_gun_name,
            (
                SELECT max(datetime(gsc_time)) from gun_settings_change
                WHERE gun_id = gun.gun_id
            ) AS last_change_time
        FROM gun, gun_settings_change as gsc, gun_settings as gst
        where
            last_change_time=gsc.gsc_time and 
            gun.gun_id = gsc.gun_id and 
            gst.gst_id = gsc.gst_id
        order by gun.gun_id
            """)
        self.inventary_gun_table.horizontalHeader().setStretchLastSection(True)
        model.setHeaderData(0, Qt.Horizontal, "Номер оружия")
        model.setHeaderData(1, Qt.Horizontal, "Конфигурация")
        model.setHeaderData(2, Qt.Horizontal, "Время последней загрузки")
        self.inventary_gun_table.setWordWrap(True)
        self.inventary_gun_table.setModel(model)
        self.inventary_gun_table.resizeColumnsToContents()

    def load_config_on_gun_clicked(self):
        self.add_gun_window = AddGun(self.db)
        self.add_gun_window.show()
        


class ServerLisener:
    timeout = 3
    lock = threading.Lock()
    exit_flag = False

    def __init__(self, server_ip):
        self.serv_ip = server_ip
        self.db = QSqlDatabase.addDatabase('QSQLITE')
        self.db.setDatabaseName(db_name)
        self.db.open()
        self.db.exec("PRAGMA foreign_keys=ON") # чтоб каскадное обновление работало

    def ListenNewHits(self):
        url = f'http://{self.serv_ip}/clear_hits'
        print("ok")


    def ListenLoadedConfigs(self):
        url = f'http://{self.serv_ip}/loaded_conf'
        '''
        response = requests.get(url)
        if(response.status_code == 200):
            print('ok')
        else:
            print("SERVER NO FOUND. NO LOADED CONF")
        '''

    def loop(self):
        while not self.exit_flag:
            time.sleep(self.timeout)
            self.ListenNewHits()
            self.ListenLoadedConfigs()



if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    
    esp_listener = ServerLisener('192.168.4.1')
    esp_lst_th = threading.Thread(target=esp_listener.loop)
    esp_lst_th.start()

    ex = app.exec_()

    esp_listener.exit_flag = True
    esp_lst_th.join()

    sys.exit(ex)
