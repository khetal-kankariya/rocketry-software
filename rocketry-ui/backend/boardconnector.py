import serial, serial.tools.list_ports

sumserial = serial.Serial(port="COM6", baudrate=115200)

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
        "gnss",
        "orientation",
        "accel",
        "softwareState",
        "gasConcn",
    ]


teleData: dict[teleDataTypes,] = {
    "teamID": "",
    "timeStamp": 0,
    "packetCount": 0,
    "altitude": 0,
    "pressure": 0,
    "temperature": 0,
    "voltage": 0,
    "gnss": {"time": 0, "latitiude": 0, "longitude": 0, "altitude": 0, "sats": 0},
    "orientation": {"x": 0, "y": 0, "z": 0},
    "accel": {"x": 0, "y": 0, "z": 0},
    "softwareState": "BOOT",
    "gasConcn": 0,
}

serialReadData = ""


def read_new_telemetry_data():

    global serialReadData
    serialReadData = sumserial.readline().decode().removesuffix("\r\n")

    (
        teleData["teamID"],
        teleData["timeStamp"],
        teleData["packetCount"],
        teleData["altitude"],
        teleData["pressure"],
        teleData["temperature"],
        teleData["voltage"],
        teleData["gnss"]["time"],
        teleData["gnss"]["alt"],
        teleData["gnss"]["lat"],
        teleData["gnss"]["lng"],
        teleData["gnss"]["sats"],
        teleData["orientation"]["x"],
        teleData["orientation"]["y"],
        teleData["orientation"]["z"],
        teleData["accel"]["x"],
        teleData["accel"]["y"],
        teleData["accel"]["z"],
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
        sumserial.write(command_str)

        command_status = {"status": "success", "message": "command sent"}
    except OSError as err:
        print(err)
        command_status = {"status": "error", "message": err}
    return command_status
