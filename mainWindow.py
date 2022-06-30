from PyQt5.QtCore import Qt, QByteArray
from PyQt5.QtNetwork import QUdpSocket, QHostAddress
from PyQt5.QtWidgets import QMainWindow, QWidget, QHBoxLayout, QVBoxLayout, QSlider


class Window(QMainWindow):
    """Main Window."""

    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        #Инициализация UI
        self.setWindowTitle("FGController")
        self.resize(800, 600)

        self.centralWidget = QWidget()
        self.hzLayout = QHBoxLayout()
        self.leftVerticalLayout = QVBoxLayout()
        self.centralVerticalLayout = QVBoxLayout()
        self.rightVerticalLayout = QVBoxLayout()

        self.pitchSlider = QSlider(Qt.Vertical, self.centralWidget)
        self.pitchSlider.setMaximum(100)
        self.pitchSlider.setMinimum(-100)
        self.leftVerticalLayout.addWidget(self.pitchSlider)

        self.throttleSlider = QSlider(Qt.Vertical, self.centralWidget)
        self.throttleSlider.setMaximum(100)
        self.throttleSlider.setMinimum(-100)
        self.rightVerticalLayout.addWidget(self.throttleSlider)

        self.rollSlider = QSlider(Qt.Horizontal, self.centralWidget)
        self.rollSlider.setMaximum(100)
        self.rollSlider.setMinimum(-100)
        self.centralVerticalLayout.addWidget(self.rollSlider)

        self.yawSlider = QSlider(Qt.Horizontal, self.centralWidget)
        self.yawSlider.setMaximum(100)
        self.yawSlider.setMinimum(-100)
        self.centralVerticalLayout.addWidget(self.yawSlider)

        self.hzLayout.addLayout(self.leftVerticalLayout)
        self.hzLayout.addLayout(self.centralVerticalLayout)
        self.hzLayout.addLayout(self.rightVerticalLayout)

        self.centralWidget.setLayout(self.hzLayout)

        self.setCentralWidget(self.centralWidget)

        #Подключение событий к методам
        self.pitchSlider.valueChanged[int].connect(self.__pitchValueChanged)

        #Инициализируем значения контроллеров
        self.pitch = 0

        #Создаем сервер
        self.socket = QUdpSocket()
        self.socket.bind(QHostAddress.Any, 5600)
        self.socket.readyRead.connect(self.__recieveData)

        self.variables = {}

    #В xml файле указан параметр "var separator = ;" и "line separator = "\n"". Они использутся при формировании строки для отправки
    #Пример var1 + ";" + var2 + "\n"
    #Порядок переменных должен совпадать с порядком чанков в input.xml
    def prepareData(self):
        return (str(self.pitch) + "\n").encode('utf-8')

    #Отправка данных
    def send(self, ip, port):
        data = self.prepareData()
        self.socket.writeDatagram(data, QHostAddress(ip), port)

    #Слот отрабатываемый при изменении значений в слайдере
    def __pitchValueChanged(self, value):
        self.pitch = value / 100
        print(self.pitch)
        self.send("127.0.0.1", 5500)

    def __recieveData(self):
        while self.socket.hasPendingDatagrams():
            datagram = self.socket.receiveDatagram()
            self.__processData(datagram)

    def __processData(self, data):
        data = data.data().data().decode("utf-8")
        splitted = data.strip('\n').split(';')
        for var in splitted:
            split = var.split(' ')
            self.variables[split[0]] = split[1]




