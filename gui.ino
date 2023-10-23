import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, QHBoxLayout, QCheckBox
from PyQt5.QtGui import QColor, QPalette

class SmartGardenGUI(QWidget):
    def _init_(self):
        super()._init_()

        self.setWindowTitle("Smart Vertical Farming")
        self.setGeometry(100, 100, 400, 200)

        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        # Add a label for the entire system
        system_label = QLabel("Smart Garden System Controls:")
        system_label.setStyleSheet("font-size: 18px; color: #333;")

        layout.addWidget(system_label)

        water_pump_checkbox_1 = QCheckBox("Water Pump (System 1)")
        led_checkbox_1 = QCheckBox("LED (System 1)")
        water_pump_checkbox_2 = QCheckBox("Water Pump (System 2)")
        led_checkbox_2 = QCheckBox("LED (System 2)")

        # Add a single Peltier Device checkbox for the whole system
        peltier_device_checkbox = QCheckBox("Humidity to Water Conversion")
        
        # Apply custom styles
        self.set_checkbox_colors(water_pump_checkbox_1, QColor(0, 128, 0))
        self.set_checkbox_colors(led_checkbox_1, QColor(0, 0, 255))
        self.set_checkbox_colors(water_pump_checkbox_2, QColor(0, 128, 0))
        self.set_checkbox_colors(led_checkbox_2, QColor(0, 0, 255))
        self.set_checkbox_colors(peltier_device_checkbox, QColor(255, 0, 0))

        layout.addWidget(water_pump_checkbox_1)
        layout.addWidget(led_checkbox_1)
        layout.addWidget(water_pump_checkbox_2)
        layout.addWidget(led_checkbox_2)
        layout.addWidget(peltier_device_checkbox)

        # Connect checkboxes to control functions
        water_pump_checkbox_1.clicked.connect(lambda: self.control_water_pump(1, water_pump_checkbox_1.isChecked()))
        led_checkbox_1.clicked.connect(lambda: self.control_led(1, led_checkbox_1.isChecked()))
        water_pump_checkbox_2.clicked.connect(lambda: self.control_water_pump(2, water_pump_checkbox_2.isChecked()))
        led_checkbox_2.clicked.connect(lambda: self.control_led(2, led_checkbox_2.isChecked()))
        peltier_device_checkbox.clicked.connect(lambda: self.control_peltier(peltier_device_checkbox.isChecked()))

        self.setLayout(layout)

    def set_checkbox_colors(self, checkbox, color):
        palette = QPalette()
        palette.setColor(QPalette.Active, QPalette.WindowText, color)
        checkbox.setPalette(palette)

    def control_water_pump(self, system, state):
        if system == 1:
            # GPIO control for Water Pump of System 1
            pass
        elif system == 2:
            # GPIO control for Water Pump of System 2
            pass

    def control_led(self, system, state):
        if system == 1:
            # GPIO control for LED of System 1
            pass
        elif system == 2:
            # GPIO control for LED of System 2
            pass

    def control_peltier(self, state):
        # GPIO control for Peltier Device
        pass

if _name_ == "_main_":
    app = QApplication(sys.argv)

    # Initialize your hardware connections here

    gui = SmartGardenGUI()
    gui.show()

    sys.exit(app.exec_())
