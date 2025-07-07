import random
from flask import Flask, jsonify
from flask_socketio import SocketIO
from flask_cors import CORS
import time
import threading
import csv
import logging

import boardconnector
from boardconnector import sim_mode, teleDataTypes

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

prevdata = {
    "teamID": "2024-ASI-ROCKETRY-026",
    "timeStamp": 0,
    "packetCount": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "voltage": 0,
    "gnss_time": 0,
    "gnss_latitude": 0,
    "gnss_longitude": 0,
    "gnss_altitude": 0,
    "gnss_sats": 0,
    "orientation_x": 0,
    "orientation_y": 0,
    "orientation_z": 0,
    "accel_x": 0,
    "accel_y": 0,
    "accel_z": 0,
    "softwareState": "BOOT",
    "gasConcn": 0,
    "velocity": 0,
}

newdata = prevdata


all_previous_data = []

csv_write_format = (
    "<TEAM ID>, <TIME STAMPING>, <PACKET COUNT>, <ALTITUDE>,"
    "<PRESSURE>, <TEMP>, <VOLTAGE>, <GNSS TIME>, <GNSS LATITUDE>,"
    "<GNSS LONGITUDE>, <GNSS ALTITUDE>, <GNSS SATS>, <ACCELEROMETER DATA>,"
    "<GYRO SPIN RATE>, <FLIGHT SOFTWARE STATE>, <ANY OPTIONAL DATA>"
)

telemetry_running = False
calibration_done = False

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
    if telemetry_running:
        global prevdata
        if sim_mode:
            newdata = random_telemetry_data()
            newdata["velocity"] = newdata["altitude"] - prevdata["altitude"]
        else:
            newdata = boardconnector.read_new_telemetry_data()
        prevdata = newdata
        global all_previous_data
        write_csv_data(newdata)
        all_previous_data.append(newdata)

        print(newdata)
        return newdata


def random_telemetry_data():
    newdata = {
        "teamID": "2024-ASI-ROCKETRY-026",
        "timeStamp": 0,
        "packetCount": (
            prevdata["packetCount"] + 1 if prevdata["packetCount"] < 255 else 0
        ),
        "altitude": int(random.random() * 51),
        "pressure": int(random.random() * 51),
        "temperature": int(random.random() * 51),
        "voltage": int(random.random() * 51),
        "gnss_time": int(random.random() * 51),
        "gnss_latitude": 23.1293,
        "gnss_longitude": 72.5417,
        "gnss_altitude": int(random.random() * 51),
        "gnss_sats": int(random.random() * 51),
        "accel_x": int(random.random() * 51),
        "accel_y": int(random.random() * 51),
        "accel_z": int(random.random() * 51),
        "orientation_x": (
            prevdata["orientation_x"] + 1 if prevdata["orientation_x"] < 359 else 0
        ),
        "orientation_y": (
            prevdata["orientation_y"] + 1 if prevdata["orientation_y"] < 359 else 0
        ),
        "orientation_z": (
            prevdata["orientation_z"] + 1 if prevdata["orientation_z"] < 359 else 0
        ),
        "softwareState": "BOOT",
        "gasConcn": int(random.random() * 51),
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
            socketio.emit("datasent", new_data, callback=datasent_callback)
        time.sleep(1)


def datasent_callback():
    print("data sent!")


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
        f"{newdata['gnss_time']},"
        f"{newdata['gnss_latitude']},"
        f"{newdata['gnss_longitude']},"
        f"{newdata['gnss_altitude']},"
        f"{newdata["gnss_sats"]},"
        f"{newdata['accel_x']},"
        f"{newdata['accel_y']},"
        f"{newdata['accel_z']},"
        f"{newdata['orientation_x']},"
        f"{newdata['orientation_y']},"
        f"{newdata['orientation_z']},"
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


@socketio.on("get-all-prevdata")
def get_all_prevdata():
    global all_previous_data
    return {"data": all_previous_data}


@socketio.on("get-startupdata")
def get_startupdata():
    return {
        "startupdata": {
            "startTime": int(starttime),
        },
    }


@socketio.on("toggle-telemetry")
def toggle_telemetry(telemetry_state: bool):
    command_status = {
        "status": "yet to send",
        "telemetry_running": False,
        "message": "none",
    }
    global telemetry_running
    if telemetry_state == False:
        telemetry_running = True
        commandsend_status = send_command("telestart")
        if commandsend_status["status"] == "success":
            command_status = {
                "status": "success",
                "telemetry_running": True,
                "message": "telemetry started",
            }
        else:
            command_status = {
                "status": "failure",
                "telemetry_running": False,
                "message": "could not start telemetry",
            }
    else:
        telemetry_running = False
        commandsend_status = send_command("telestop")
        if commandsend_status["status"] == "success":
            command_status = {
                "status": "success",
                "telemetry_running": False,
                "message": "telemetry stopped",
            }
        else:
            command_status = {
                "status": "failure",
                "telemetry_running": True,
                "message": "could not stop telemetry",
            }
    return command_status


@socketio.on("get-telemetry-state")
def get_telemetry_state():
    global telemetry_running
    return telemetry_running


@socketio.on("get-calibration-state")
def get_calibration_state():
    global calibration_done
    return calibration_done


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
            "status": "failure",
            "message": "could not calibrate sensors",
        }

        print("failure")
        return calistart_status
    else:
        calistart_status = {"status": "success", "message": "calibration started"}

    # CHECK CALIBRATION
    def calibration_checker():
        timern = time.time()
        calibrated = True
        while time.time() - timern < 6:
            if float(newdata["altitude"]) > 1:
                time.sleep(1)
                pass
            else:
                calibrated = True
                break

        if calibrated:
            return {"status": "success", "message": "sensors calibrated"}
        else:
            return {
                "status": "failure",
                "message": "could not calibrate sensors",
            }

    calibration_status = calibration_checker()
    global calibration_done
    calibration_done = True
    return calibration_status


def send_command(command_str: str):
    if sim_mode:
        command_status = {"status": "success", "message": "command sent"}
    else:
        command_status = boardconnector.send_command(command_str)
    return command_status


if __name__ == "__main__":
    logging.log(logging.INFO, "Server started")
    socketio.run(app, port=5000)
