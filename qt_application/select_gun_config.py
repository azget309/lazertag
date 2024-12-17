import sys
import os
import requests
import json

# from widgets
from PySide6 import QtCore, QtGui, QtWidgets
from PySide6.QtUiTools import loadUiType
from PySide6.QtSql import *

import c_gun_config


select_gun_config_generate_class, select_gun_config_base_class = loadUiType("ui/select_gun_config.ui")


class AddGun(select_gun_config_base_class, select_gun_config_generate_class):
    def __init__(self, db):
        super().__init__()
        select_gun_config_generate_class.__init__(self)
        self.setupUi(self)
        self.id = 0

        self.db = db

        self.dialog_button.accepted.connect(self.ok_clicked)
        self.dialog_button.rejected.connect(self.reject_clicked)


        self.select_gun_config_list.clear()
        item = QtWidgets.QListWidgetItem("УДАЛИТЬ ОРУЖЕЕ ИЗ БАЗЫ")
        item.setData(QtCore.Qt.UserRole, -1)
        self.select_gun_config_list.addItem(item)
        query = QSqlQuery("select gst_gun_name, gst_id from gun_settings where gst_is_deleted=0;")
        while query.next():
            gst_gun_name = query.value(query.record().indexOf('gst_gun_name'))
            gst_id = query.value(query.record().indexOf('gst_id'))
            item = QtWidgets.QListWidgetItem(gst_gun_name)
            item.setData(QtCore.Qt.UserRole, gst_id)
            self.select_gun_config_list.addItem(item)
        self.select_gun_config_list.setCurrentRow(0)
    
    def config_to_json(self, conf_id, gun_id):
        sql = f"SELECT * FROM gun_settings WHERE gst_id = {conf_id} LIMIT 1"
        query = QSqlQuery()
        query.setForwardOnly(True)
        if not query.exec(sql):
            print("err")
            return ""

        conf = {'gun_id':gun_id}
        while query.next():
            for i in range(query.record().count()):
                key = query.record().fieldName(i)
                val = query.value(i)
                conf[key] = val
        return json.dumps(conf)
    
    def ok_clicked(self):
        msg = QtWidgets.QMessageBox()
        msg.setText("""Для загрузки конфигурации в оружие выстрелете им в пульт управления""")
        msg.setWindowTitle("Info")
        msg.exec()
        conf_id = self.select_gun_config_list.currentItem().data(QtCore.Qt.UserRole)
        gun_id  = str(self.select_gun_id_spin_box.value())
        if conf_id == -1:
            #QSqlQuery(f"DELETE FROM gun WHERE gun_id = '{gun_id}'").exec()
            print(f"deleted conf from {gun_id}")
        else:
            json_conf = self.config_to_json(conf_id, gun_id)
            url = 'http://192.168.239.233/load_conf'
            response = requests.post(url, json=json.loads(json_conf))
            if response.status_code == 200:
                print(response.text)
                print('конфа прилетела на пульт')
            else:
                print(response.text)
                print('нет подключения к пульту')
        
    
    def reject_clicked(self):
        print("reject")

