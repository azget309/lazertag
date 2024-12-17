import sys
import os

# from widgets
from PySide6 import QtCore, QtGui, QtWidgets
from PySide6.QtUiTools import loadUiType
from PySide6.QtPrintSupport import QPrinter

# from database
from PySide6.QtWebEngineWidgets import QWebEngineView 
from PySide6.QtCore import QUrl
from PySide6.QtWidgets import QApplication, QMainWindow, QFileDialog
from PySide6.QtSql import *


# from report
import plotly 
from plotly.offline import iplot
plotly.offline.init_notebook_mode(connected=True)
import plotly.io as pio
import plotly.graph_objects as go 
from plotly.subplots import make_subplots

from select_gun_config import AddGun

from report import *

sessions_generated_class, sessions_base_class = loadUiType("ui/sessions.ui")
report_generated_class, report_base_class = loadUiType("ui/report.ui")



class ReportApp(report_base_class, report_generated_class):
    def __init__(self, db, ses_id):
        super().__init__()
        report_generated_class.__init__(self)
        self.setupUi(self)

        self.html = GenerateHtmlReport(db, ses_id)

        self.plot_widget = QWebEngineView()
        self.plot_widget.setHtml(
            self.html,
            baseUrl=QUrl.fromLocalFile(os.path.dirname(os.path.realpath(__file__)))
        )
        
        gb_layout = QVBoxLayout()
        gb_layout.addWidget(self.plot_widget)
        self.report_group_box.setLayout(gb_layout)
        
        self.pdf_button.clicked.connect(self.pdf_button_click)

    def pdf_button_click(self):
        file = QFileDialog.getSaveFileName(self, 'Save File')
        
        loader = QWebEngineView()
        loader.setHtml(
            self.html,
            baseUrl=QUrl.fromLocalFile(os.path.dirname(os.path.realpath(__file__)))
        )
        def tmp(finished):
            loader.show()
            layout = QPageLayout()
            layout.setPageSize(QPageSize(QPageSize.A4Extra))
            layout.setOrientation(QPageLayout.Landscape)
            loader.page().printToPdf(file[0], layout)
        loader.loadFinished.connect(tmp)
            



class SessionsApp(sessions_base_class, sessions_generated_class):
    selected_ses_id = -1
    def __init__(self):
        super().__init__()
        sessions_generated_class.__init__(self)
        self.setupUi(self)

        self.db = QSqlDatabase.addDatabase('QSQLITE')
        self.db.setDatabaseName('lazertag_db.sqlite')
        self.db.open()
        self.db.exec("PRAGMA foreign_keys=ON") # чтоб каскадное обновление работало
        
        self.players_model = QSqlRelationalTableModel()
        self.players_model.setTable('player')
        self.players_model.setJoinMode(QSqlRelationalTableModel.LeftJoin)
        self.players_model.setHeaderData(self.players_model.fieldIndex('plr_name'), Qt.Horizontal, "Игрок")
        self.players_model.setHeaderData(self.players_model.fieldIndex('det_id'), Qt.Horizontal, "Номер датчика")

        self.players_table_view.horizontalHeader().setStretchLastSection(True)
        self.players_table_view.setWordWrap(True)
        self.players_table_view.setModel(self.players_model)
        self.players_table_view.setColumnHidden(self.players_model.fieldIndex('ses_id'),True)

        self.ses_start_date_edit.setDisplayFormat("yyyy-MM-dd hh:mm:ss")
        self.ses_time_edit.setDisplayFormat("hh:mm:ss")

        self.update_sessions_list()

        self.config_connect()

    def config_connect(self):
        self.sessions_list_widget.itemSelectionChanged.connect(self.select_session)
        self.add_player_button.clicked.connect(self.add_player_button_click)
        self.remove_player_button.clicked.connect(self.remove_player_button_click)
        self.new_session_button.clicked.connect(self.new_session_button_click)
        self.generate_report_button.clicked.connect(self.generate_report_button_click)
        self.ses_start_date_edit.dateTimeChanged.connect(self.ses_start_date_changed)
        self.ses_time_edit.dateTimeChanged.connect(self.ses_time_changed)
    
    
    def select_session(self):
        sl_ses_id = self.sessions_list_widget.currentItem().data(QtCore.Qt.UserRole)
        
        self.selected_ses_id = sl_ses_id
        self.date_time_group_box.setEnabled(True)
        self.players_group_box.setEnabled(True)
        self.generate_report_button.setEnabled(True)
        self.update_players_list()
        self.update_date_edit()


    def update_cur_session_value(self, val_name, val_value):
        QSqlQuery(f"\
            UPDATE play_session\
            SET {val_name} = \"{val_value}\"\
            WHERE ses_id = \"{self.selected_ses_id}\"").exec()


    def ses_start_date_changed(self, date):
        self.update_cur_session_value("ses_start_date", date.toString("yyyy-MM-dd hh:mm:ss"))
        self.update_sessions_list()
        print(date.toString("yyyy-MM-dd hh:mm:ss"))

    def ses_time_changed(self, time):
        self.update_cur_session_value("ses_session_time", time.toString("hh:mm:ss"))
        self.update_sessions_list()
        print(time.toString("hh:mm:ss"))


    def update_date_edit(self):
        self.ses_start_date_edit.blockSignals(True)
        self.ses_time_edit.blockSignals(True)
        sql = f'select * from play_session where ses_id={self.selected_ses_id}'
        query = QSqlQuery(sql)
        while query.next():
            ses_start_date = query.value(query.record().indexOf('ses_start_date'))
            ses_session_time = query.value(query.record().indexOf('ses_session_time'))
            self.ses_start_date_edit.setDateTime(QDateTime.fromString(ses_start_date, "yyyy-MM-dd hh:mm:ss"))
            self.ses_time_edit.setTime(QTime.fromString(ses_session_time, "hh:mm:ss"))
            break;
        self.ses_start_date_edit.blockSignals(False)
        self.ses_time_edit.blockSignals(False)
    
    def update_players_list(self):
        self.players_model.setFilter(f"ses_id = '{self.selected_ses_id}'")
        self.players_model.select()
        self.players_table_view.resizeColumnsToContents()

    def update_sessions_list(self):
        self.sessions_list_widget.blockSignals(True)
        self.sessions_list_widget.clear()
        sql = 'select * from play_session'
        query = QSqlQuery(sql)
        old_item = False
        while query.next():
            ses_id = query.value(query.record().indexOf('ses_id'))
            ses_start_date = query.value(query.record().indexOf('ses_start_date'))
            ses_session_time = query.value(query.record().indexOf('ses_session_time'))
            item = QListWidgetItem("сеанс от "+ses_start_date)
            item.setData(QtCore.Qt.UserRole, ses_id)
            self.sessions_list_widget.addItem(item)
            if(ses_id == self.selected_ses_id):
                old_item = item
        self.sessions_list_widget.blockSignals(False)

        if(not old_item):
            self.selected_ses_id = -1
            self.date_time_group_box.setEnabled(False)
            self.players_group_box.setEnabled(False)
            self.generate_report_button.setEnabled(False)
        else:
            self.sessions_list_widget.setCurrentItem(old_item)

    def new_session_button_click(self):
        now_time = QDateTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        sql = f"INSERT INTO play_session (ses_start_date, ses_session_time)\
                VALUES ('{now_time}', '00:00:00');"
        query = QSqlQuery(sql)
        self.update_sessions_list()
        print("new ses")
    
    def add_player_button_click(self):
        row = self.players_model.rowCount()
        ses_id_index = self.players_model.fieldIndex('ses_id')
        self.players_model.insertRow(row)
        self.players_model.setData(
                self.players_model.index(row, ses_id_index),
                self.selected_ses_id)
        print("add plr")

    def remove_player_button_click(self):
        idx = self.players_table_view.currentIndex().row()
        if(idx >= 0):
            self.players_model.removeRows(idx, 1)
            print("rm plr")
        else:
            msg = QtWidgets.QMessageBox()
            msg.setText("Игрок не выбран")
            msg.setWindowTitle("Error")
            msg.exec()

    def generate_report_button_click(self):
        self.report_win = ReportApp(self.db, self.selected_ses_id)
        self.report_win.show()

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = SessionsApp()
    window.show()
    #db.close()
    sys.exit(app.exec_())
