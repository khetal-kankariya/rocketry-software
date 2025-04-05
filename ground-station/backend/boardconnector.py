import serial
import serial.tools.list_ports

sumserial = serial.Serial(port="COM13", baudrate=115200)

serialPorts = serial.tools.list_ports.comports()

serialBaudrate = sumserial.baudrate
print("baud rate: ", serialBaudrate)

serialports = []

print("ports available: ")
for port, desc, hwid in sorted(serialPorts):
    if desc != "n/a":
        serialports.append(port)
        print("\t", port, desc)


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
    ]


teleData: dict[teleDataTypes,] = {
    "teamID": "",
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
}


def read_new_telemetry_data():

    serialReadData = sumserial.readline().decode().removesuffix("\r\n")
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
        teleData["gnss_alt"],
        teleData["gnss_lat"],
        teleData["gnss_lng"],
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

    print(teleData)

    return teleData


def get_all_comports():
    global ports
    return ports


def get_baudrate():
    global serialBaudrate
    return serialBaudrate


def send_command(command_str: str):
    command_status = {"status": "none", "message": "none"}
    try:
        sumserial.write(command_str.encode())

        command_status = {"status": "success", "message": "command sent"}
    except OSError as err:
        print(err)
        command_status = {"status": "error", "message": err}
    return command_status
