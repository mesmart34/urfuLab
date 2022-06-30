from PyQt5.QtWidgets import QMainWindow, QApplication

from mainWindow import Window

if __name__ == '__main__':
    app = QApplication([])
    window = Window()
    window.show()
    app.exec()
