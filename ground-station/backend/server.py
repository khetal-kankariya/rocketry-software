import random
from flask import Flask, jsonify
from flask_socketio import SocketIO
from flask_cors import CORS
import time
import threading
import csv
import logging
from typing import Union

import boardconnector
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

prev_rocket_telemetry: boardconnector.TrocketField = {
    "team_id": "2024-ASI-ROCKETRY-026",
    "time_stamp": "00:00:00",
    "packet_count": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "voltage": 0,
    "gnss_time": "00:00:00",
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
    "software_state": "BOOT",
    "gas_concentration": 0,
    "velocity": 0,
}

prev_payload_telemetry: boardconnector.TpayloadField = {
    "team_id": "2024-ASI-ROCKETRY-026",
    "packet_count": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "orientation_x": 0,
    "orientation_y": 0,
    "orientation_z": 0,
    "accel_x": 0,
    "accel_y": 0,
    "accel_z": 0,
    "gas_concentration": 0,
    "velocity": 0,
}

rocket_telemetry = prev_rocket_telemetry
payload_telemetry = prev_payload_telemetry

all_previous_data = []

csv_write_format = (
    "<TEAM ID>, <TIME STAMPING>, <PACKET COUNT>, <ALTITUDE>,"
    "<PRESSURE>, <TEMP>, <VOLTAGE>, <GNSS TIME>, <GNSS LATITUDE>,"
    "<GNSS LONGITUDE>, <GNSS ALTITUDE>, <GNSS SATS>, <ACCELEROMETER DATA>,"
    "<GYRO SPIN RATE>, <FLIGHT SOFTWARE STATE>, <ANY OPTIONAL DATA>"
)

telemetry_rocket_running = False
telemetry_payload_running = False
rocket_calibration_done = False
payload_calibration_done = False

sensor_field = {
    "rtc": "time_stamp",
    "gnss": "gnss",
    "imu": "orientation",
    "gas": "gas_concentration",
    "pressure": "pressure",
    "logger": None,
    "temp_humid": "temperature",
    "voltage": "voltage",
}


# NEW TELEMETRY DATA AND CSV WRITING
def new_telemetry_data():
    global prev_rocket_telemetry, prev_payload_telemetry, rocket_telemetry, payload_telemetry
    if boardconnector.sim_mode:
        rocket_telemetry = random_rocket_telemetry()
        payload_telemetry = random_payload_telemetry()
    else:
        rocket_telemetry = boardconnector.read_rocket_telemetry()
        payload_telemetry = boardconnector.read_rocket_telemetry()

    if telemetry_rocket_running:
        prev_rocket_telemetry = rocket_telemetry
        prev_payload_telemetry = payload_telemetry

        global all_previous_data
        all_previous_data.append(rocket_telemetry)

        write_rocket_csv(rocket_telemetry)
        write_payload_csv(payload_telemetry)

        print(rocket_telemetry, payload_telemetry)
        return rocket_telemetry, payload_telemetry
    else:
        return None, None


def random_rocket_telemetry() -> boardconnector.TrocketField:
    newdata: boardconnector.TrocketField = {
        "team_id": "2024-ASI-ROCKETRY-026",
        "time_stamp": "00:00:00",
        "packet_count": (
            prev_rocket_telemetry["packet_count"] + 1
            if prev_rocket_telemetry["packet_count"] < 255
            else 0
        ),
        "altitude": int(random.random() * 51),
        "pressure": int(random.random() * 51),
        "temperature": int(random.random() * 51),
        "voltage": int(random.random() * 51),
        "gnss_time": "00:00:00",
        "gnss_latitude": 23.1293,
        "gnss_longitude": 72.5417,
        "gnss_altitude": int(random.random() * 51),
        "gnss_sats": int(random.random() * 51),
        "accel_x": int(random.random() * 51),
        "accel_y": int(random.random() * 51),
        "accel_z": int(random.random() * 51),
        "orientation_x": (
            prev_rocket_telemetry["orientation_x"] + 1
            if prev_rocket_telemetry["orientation_x"] < 359
            else 0
        ),
        "orientation_y": (
            prev_rocket_telemetry["orientation_y"] + 1
            if prev_rocket_telemetry["orientation_y"] < 359
            else 0
        ),
        "orientation_z": (
            prev_rocket_telemetry["orientation_z"] + 1
            if prev_rocket_telemetry["orientation_z"] < 359
            else 0
        ),
        "software_state": "BOOT",
        "gas_concentration": int(random.random() * 51),
        "velocity": 0,
    }

    return newdata


def random_payload_telemetry() -> boardconnector.TpayloadField:
    newdata: boardconnector.TpayloadField = {
        "team_id": "2024-ASI-ROCKETRY-026",
        "packet_count": (
            prev_rocket_telemetry["packet_count"] + 1
            if prev_rocket_telemetry["packet_count"] < 255
            else 0
        ),
        "altitude": int(random.random() * 51),
        "pressure": int(random.random() * 51),
        "temperature": int(random.random() * 51),
        "accel_x": int(random.random() * 51),
        "accel_y": int(random.random() * 51),
        "accel_z": int(random.random() * 51),
        "orientation_x": (
            prev_rocket_telemetry["orientation_x"] + 1
            if prev_rocket_telemetry["orientation_x"] < 359
            else 0
        ),
        "orientation_y": (
            prev_rocket_telemetry["orientation_y"] + 1
            if prev_rocket_telemetry["orientation_y"] < 359
            else 0
        ),
        "orientation_z": (
            prev_rocket_telemetry["orientation_z"] + 1
            if prev_rocket_telemetry["orientation_z"] < 359
            else 0
        ),
        "gas_concentration": int(random.random() * 51),
        "velocity": 0,
    }

    return newdata


def send_telemetry_data():
    while True:
        new_rocket_data, new_payload_data = new_telemetry_data()
        if telemetry_rocket_running and telemetry_payload_running:
            socketio.emit(
                "datasent",
                {"rocket": new_rocket_data, "payload": new_payload_data},
                callback=datasent_callback,
            )
        time.sleep(1)


def datasent_callback():
    print("data sent!")


# WRITING CSV DATA
def write_rocket_csv(data: boardconnector.TrocketField):
    csv_write_file = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026-rocket.csv", mode="a+"
    )
    csv_write_file.write(
        f"{rocket_telemetry['team_id']},"
        f"{rocket_telemetry['time_stamp']},"
        f"{rocket_telemetry['packet_count']},"
        f"{rocket_telemetry['software_state']},"
        f"{rocket_telemetry['altitude']},"
        f"{rocket_telemetry['pressure']},"
        f"{rocket_telemetry['temperature']},"
        f"{rocket_telemetry['voltage']},"
        f"{rocket_telemetry['gnss_time']},"
        f"{rocket_telemetry['gnss_latitude']},"
        f"{rocket_telemetry['gnss_longitude']},"
        f"{rocket_telemetry['gnss_altitude']},"
        f"{rocket_telemetry["gnss_sats"]},"
        f"{rocket_telemetry['accel_x']},"
        f"{rocket_telemetry['accel_y']},"
        f"{rocket_telemetry['accel_z']},"
        f"{rocket_telemetry['orientation_x']},"
        f"{rocket_telemetry['orientation_y']},"
        f"{rocket_telemetry['orientation_z']},"
        f"{rocket_telemetry['gas_concentration']},"
        f"{rocket_telemetry['velocity']}\n"
    )
    csv_write_file.close()


def write_payload_csv(data: boardconnector.TpayloadField):
    csv_write_file = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026-payload.csv", mode="a+"
    )
    csv_write_file.write(
        f"{rocket_telemetry['team_id']},"
        f"{rocket_telemetry['time_stamp']},"
        f"{rocket_telemetry['packet_count']},"
        f"{rocket_telemetry['software_state']},"
        f"{rocket_telemetry['altitude']},"
        f"{rocket_telemetry['pressure']},"
        f"{rocket_telemetry['temperature']},"
        f"{rocket_telemetry['voltage']},"
        f"{rocket_telemetry['gnss_time']},"
        f"{rocket_telemetry['gnss_latitude']},"
        f"{rocket_telemetry['gnss_longitude']},"
        f"{rocket_telemetry['gnss_altitude']},"
        f"{rocket_telemetry["gnss_sats"]},"
        f"{rocket_telemetry['accel_x']},"
        f"{rocket_telemetry['accel_y']},"
        f"{rocket_telemetry['accel_z']},"
        f"{rocket_telemetry['orientation_x']},"
        f"{rocket_telemetry['orientation_y']},"
        f"{rocket_telemetry['orientation_z']},"
        f"{rocket_telemetry['gas_concentration']},"
        f"{rocket_telemetry['velocity']}\n"
    )
    csv_write_file.close()


threading.Thread(target=send_telemetry_data).start()


# READING AND SENDING CSV DATA ON DEMAND
@socketio.on("get-csv-data")
def get_csv_data():
    csv_readdata: dict[str, list[list[str]]] = {"rocket": [], "payload": []}

    csv_read_file__rocket = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026-rocket.csv", mode="r"
    )
    csv_read_file__payload = open(
        newline="\n", file="../files/2024ASI-ROCKETRY-026-payload.csv", mode="r"
    )

    for line in csv.reader(csv_read_file__rocket):
        csv_readdata["rocket"].append(line)

    for line in csv.reader(csv_read_file__payload):
        csv_readdata["payload"].append(line)

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
def toggle_telemetry():
    global telemetry_rocket_running
    if telemetry_rocket_running == False:
        telemetry_rocket_running = True
        return send_command("telestart")
    else:
        telemetry_rocket_running = False
        return send_command("telestop")


@socketio.on("get-telemetry-state")
def get_telemetry_state():
    global telemetry_rocket_running
    return telemetry_rocket_running


@socketio.on("get-calibration-state")
def get_calibration_state():
    global rocket_calibration_done, payload_calibration_done
    return {"rocket": rocket_calibration_done, "payload": payload_calibration_done}


@socketio.on("calibrate-sensors")
def calibrate_sensors() -> boardconnector.Tstatus:
    command_status = send_command("calibrate")
    return command_status


def send_command(command_str: str, platform="both") -> boardconnector.Tstatus:
    if not boardconnector.sim_mode:
        if platform == "rocket":
            status_rocket = boardconnector.send_rocket_command(
                boardconnector.sim_mode, command_str
            )
            return {
                "status": status_rocket["status"],
                "rocket": status_rocket["status"],
                "payload": False,
                "message": status_rocket["message"]
                + " and No command sent for payload",
            }
        elif platform == "payload":
            status_payload = boardconnector.send_payload_command(
                boardconnector.sim_mode, command_str
            )
            return {
                "status": status_payload["status"],
                "rocket": False,
                "payload": status_payload["status"],
                "message": "No command sent for rocket and "
                + status_payload["message"],
            }
        else:
            status_rocket = boardconnector.send_rocket_command(
                boardconnector.sim_mode, command_str
            )
            status_payload = boardconnector.send_payload_command(
                boardconnector.sim_mode, command_str
            )
            return {
                "status": status_rocket["status"] and status_payload["status"],
                "rocket": status_rocket["status"],
                "payload": status_payload["status"],
                "message": status_rocket["message"]
                + " and "
                + status_payload["message"],
            }
    else:
        if command_str == "calibrate" or "telestart" or "telestop":
            return {
                "status": True,
                "rocket": True,
                "payload": True,
                "message": f"{command_str} executed successfully",
            }
        else:
            return {
                "status": False,
                "rocket": True,
                "payload": True,
                "message": "Invalid Command",
            }


if __name__ == "__main__":
    logging.log(logging.INFO, "Server started")
    socketio.run(app, port=5000)
