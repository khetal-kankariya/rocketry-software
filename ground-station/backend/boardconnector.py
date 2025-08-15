import serial
import serial.serialutil

sim_mode = False
rocket_serial = serial.Serial()
payload_serial = serial.Serial()
try:
    rocket_serial = serial.Serial(port="COM3", baudrate=115200)
    payload_serial = serial.Serial(port="COM13", baudrate=115200)
except Exception as err:
    sim_mode = True

teleFormat = (
    "<TEAM ID>, <TIME STAMPING>, <PACKET COUNT>, <ALTITUDE>, <PRESSURE>,"
    " <TEMP>, <VOLTAGE>, <GNSS TIME>, <GNSS LATITUDE>, <GNSS LONGITUDE>, "
    "<GNSS ALTITUDE>, <GNSS SATS>, <ACCELEROMETER DATA>, <GYRO SPIN RATE>, "
    "<FLIGHT SOFTWARE STATE>, <ANY OPTIONAL DATA>"
)


class teleDataTypes:
    [
        "teamID",
        "timeStamp",
        "packet_count",
        "altitude",
        "pressure",
        "temperature",
        "voltage",
        "gnss_time",
        "gnss_latitude",
        "gnss_longitude",
        "gnss_altitude",
        "gnss_sats",
        "orientation_x",
        "orientation_y",
        "orientation_z",
        "accel_x",
        "accel_y",
        "accel_z",
        "softwareState",
        "gasConcn",
        "velocity",
    ]


teleData: dict[teleDataTypes, int | float | str] = {
    "teamID": "2024-ASI-ROCKETRY-026",
    "timeStamp": "",
    "packetCount": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "voltage": 0,
    "gnss_time": "",
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


def read_new_telemetry_data():
    try:
        serialReadData = rocket_serial.readline().decode().removesuffix("\r\n")
        new_data_read = len(serialReadData)

        (
            teleData["teamID"],
            teleData["timeStamp"],
            teleData["packetCount"],
            teleData["altitude"],
            teleData["pressure"],
            teleData["temperature"],
            teleData["voltage"],
            teleData["gnss_time"],
            teleData["gnss_latitude"],
            teleData["gnss_longitude"],
            teleData["gnss_altitude"],
            teleData["gnss_sats"],
            teleData["orientation_x"],
            teleData["orientation_y"],
            teleData["orientation_z"],
            teleData["accel_x"],
            teleData["accel_y"],
            teleData["accel_z"],
            teleData["softwareState"],
            teleData["gasConcn"],
        ) = serialReadData.split(",")
    finally:
        return teleData


def send_command(command_str: str):
    command_status = {"status": "none", "message": "none"}
    try:
        rocket_serial.write(command_str.encode())

        command_status = {"status": "success", "message": "command sent"}
    except OSError as err:
        print(err)
        command_status = {"status": "error", "message": err}
    return command_status
