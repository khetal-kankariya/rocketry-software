import random
from flask import Flask, jsonify
from flask_socketio import SocketIO, send
from flask_cors import CORS
import time
import threading
import csv
import logging

import boardconnector

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
CORS(app)

logging.basicConfig(
    filename="server-log.log",
    encoding="utf-8",
    filemode="a",
    format="{asctime} - {levelname} - {message}",
    style="{",
    datefmt="%Y-%m-%d %H:%M",
)


starttime = time.time()
nowtime = 0


ports = ["COM 3", "COM 4"]
activePort = "COM 3"

baudRates = [9600, 115200]
activeBaudRate = 9600

prevdata = {
    "teamID": "2024ASI-ROCKETRY-026",
    "timeStamp": 0,
    "packetCount": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "voltage": 0,
    "gasConcn": 0,
    "gnss": {"time": 0, "lat": 0, "lng": 0, "alt": 0, "sats": 0},
    "accel": {"x": 0, "y": 0, "z": 0},
    "orientation": {"x": 0, "y": 0, "z": 0},
    "gyroSpinRate": {"roll": 0, "pitch": 0},
    "softwareState": "boot",
    "velocity": 0,
}
newdata = prevdata

csv_write_format = "<TEAM ID>, <TIME STAMPING>, <PACKET COUNT>, <ALTITUDE>, <PRESSURE>, <TEMP>, <VOLTAGE>, <GNSS TIME>, <GNSS LATITUDE>, <GNSS LONGITUDE>, <GNSS ALTITUDE>, <GNSS SATS>, <ACCELEROMETER DATA>, <GYRO SPIN RATE>, <FLIGHT SOFTWARE STATE>, <ANY OPTIONAL DATA>"

telemetry_running = False

mission_progress = 0

sensor_status = {
    "rtc": True,
    "gnss": True,
    "imu": True,
    "gas": True,
    "pressure": True,
    "logger": True,
    "temp_humid": True,
    "voltage": True,
}

sensor_field = {
    "rtc": "timeStamp",
    "gnss": "gnss",
    "imu": "orientation",
    "gas": "gasConcn",
    "pressure": "pressure",
    "logger": None,
    "temp_humid": "temperature",
    "voltage": "voltage",
}


# NEW TELEMETRY DATA AND CSV WRITING
def new_telemetry_data():

    global nowtime, newdata, prevdata
    nowtime = time.time()
    # newdata = random_telemetry_data()
    newdata = boardconnector.read_new_telemetry_data()
    prevdata = newdata

    # write_csv_data(newdata)
    return newdata


def random_telemetry_data():
    newdata = {
        "teamID": "2024ASI-ROCKETRY-026",
        "timeStamp": int(nowtime - starttime),
        "packetCount": (
            prevdata["packetCount"] + 1 if prevdata["packetCount"] < 255 else 0
        ),
        "altitude": int(random.random() * 51),
        "pressure": int(random.random() * 51),
        "temperature": int(random.random() * 51),
        "voltage": int(random.random() * 51),
        "gasConcn": int(random.random() * 51),
        "gnss": {
            "time": int(random.random() * 51),
            "lat": 23.1293,
            "lng": 72.5417,
            "alt": int(random.random() * 51),
            "sats": int(random.random() * 51),
        },
        "accel": {
            "x": int(random.random() * 51),
            "y": int(random.random() * 51),
            "z": int(random.random() * 51),
        },
        "orientation": {
            "x": (
                prevdata["orientation"]["x"] + 1
                if prevdata["orientation"]["x"] < 359
                else 0
            ),
            "y": (
                prevdata["orientation"]["y"] + 1
                if prevdata["orientation"]["y"] < 359
                else 0
            ),
            "z": (
                prevdata["orientation"]["z"] + 1
                if prevdata["orientation"]["z"] < 359
                else 0
            ),
        },
        "gyroSpinRate": {
            "roll": int(random.random() * 51),
            "pitch": int(random.random() * 51),
        },
        "softwareState": "boot",
        "velocity": 0,
    }

    newdata["velocity"] = (newdata["altitude"] - prevdata["altitude"]) / (
        max(newdata["timeStamp"] - prevdata["timeStamp"], 0.5)
    )

    return newdata


def send_telemetry_data():
    while True:
        new_data = new_telemetry_data()
        if telemetry_running:
            socketio.emit("datasent", new_data)
        time.sleep(1)


# WRITING CSV DATA
def write_csv_data(data: dict[str, any]):
    csv_write_file = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026.csv", mode="a+"
    )
    csv_write_file.write(
        f"{newdata['teamID']},"
        f"{newdata['timeStamp']},"
        f"{newdata['packetCount']},"
        f"{newdata['softwareState']},"
        f"{newdata['altitude']},"
        f"{newdata['pressure']},"
        f"{newdata['temperature']},"
        f"{newdata['voltage']},"
        f"{newdata['gnss']['time']},"
        f"{newdata['gnss']['lat']},"
        f"{newdata['gnss']['lng']},"
        f"{newdata['gnss']['alt']},"
        f"{newdata['gnss']["sats"]},"
        f"{newdata['accel']['x']},"
        f"{newdata['accel']['y']},"
        f"{newdata['accel']['z']},"
        f"{newdata['orientation']['x']},"
        f"{newdata['orientation']['y']},"
        f"{newdata['orientation']['z']},"
        f"{newdata['gyroSpinRate']['roll']},"
        f"{newdata['gyroSpinRate']['pitch']},"
        f"{newdata['gasConcn']},"
        f"{newdata['velocity']}\n"
    )
    csv_write_file.close()


threading.Thread(target=send_telemetry_data).start()


# READING AND SENDING CSV DATA ON DEMAND
@socketio.on("get-csv-data")
def get_csv_data():
    csv_readdata = []

    csv_read_file = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026.csv", mode="r"
    )

    for line in csv.reader(csv_read_file):
        csv_readdata.append(line)

    return {"data": csv_readdata}


@socketio.on("get-startupdata")
def get_startupdata():
    return {
        "startupdata": {
            "startTime": int(starttime),
            "selectedPort": activePort,
            "baudRateList": baudRates,
        },
    }


@socketio.on("toggle-telemetry")
def toggle_telemetry(telemetry_state: bool):
    telemetry_status = {"status": "yet to send", "message": "none"}
    global telemetry_running
    if telemetry_state == False:
        telemetry_running = True
        commandsend_status = send_command("telestart")
        if commandsend_status["status"] == "success":
            telemetry_status = {"status": "success", "message": "telemetry started"}
        else:
            telemetry_status = {
                "status": "failed",
                "message": "could not start telemetry",
            }
    else:
        telemetry_running = False
        commandsend_status = send_command("telestop")
        if commandsend_status["status"] == "success":
            telemetry_status = {"status": "success", "message": "telemetry stopped"}
        else:
            telemetry_status = {
                "status": "failed",
                "message": "could not stop telemetry",
            }
    print(telemetry_status)
    return telemetry_running


@socketio.on("get-telemetry-state")
def get_telemetry_state():
    global telemetry_running
    return telemetry_running


@socketio.on("get-mission-progress")
def get_mission_progress():
    return mission_progress


@socketio.on("calibrate-sensors")
def calibrate_sensors():
    calistart_status = {"status": "yet to send", "message": "none"}
    calibration_status = {
        "status": "not calibrated",
        "message": "calibration command yet not sent",
    }

    commandsend_status = send_command("calibrate")

    if commandsend_status["status"] != "success":
        calistart_status = {
            "status": "failed",
            "message": "could not calibrate sensors",
        }
        return calistart_status
    else:
        print(commandsend_status)
        calistart_status = {"status": "success", "message": "calibration started"}

    # CHECK CALIBRATION
    def calibration_checker():
        timern = time.time()
        calibrated = False
        while time.time() - timern < 6:
            if newdata["altitude"] > 1:
                time.sleep(1)
                pass
            else:
                calibrated = True
                break

        if calibrated:
            return {"status": "calibrated", "message": "sensors calibrated"}
        else:
            return {
                "status": "not calibrated",
                "message": "could not calibrate sensors",
            }

    calibration_status = calibration_checker()

    return calibration_status


def send_command(command_str: str):
    command_status = {"status": "success", "message": "command sent"}
    # command_status = boardconnector.send_command(command_str)
    return command_status


if __name__ == "__main__":
    logging.log(logging.INFO, "Server started")
    socketio.run(app)
