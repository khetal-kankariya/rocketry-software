import serial
import serial.tools.list_ports
from typing import TypedDict
print(serial.tools.list_ports.main())

sim_mode = False
rocket_serial = serial.Serial()
payload_serial = serial.Serial()
rocket_port = 3
payload_port = 13



inp = str(input("Enter Simulation Mode? [Y/n]: "))
if (inp == "n" or "N"):
    try:
        rocket_port = int(input("Enter rocket xbee port number: "))
        print(f"COM{rocket_port}")
        rocket_serial = serial.Serial(port="/dev/ttyACM0", baudrate=115200)
    except Exception as err:
        print(f"Can not Read Serial Port {rocket_port}")
    # try:
    #     # payload_port = int(input("Enter payload xbee port number: "))
    #     # payload_serial = serial.Serial(port=f"COM{payload_serial}", baudrate=115200)
    # except Exception as err:
    #     print(f"Can not Read Serial Port {payload_port}")
else:
    sim_mode = True


class TrocketField(TypedDict):
    team_id: str
    time_stamp: str
    packet_count: int
    altitude: float
    pressure: float
    temperature: float
    voltage: float
    gnss_time: str
    gnss_latitude: float
    gnss_longitude: float
    gnss_altitude: float
    gnss_sats: int
    orientation_x: float
    orientation_y: float
    orientation_z: float
    accel_x: float
    accel_y: float
    accel_z: float
    software_state: str
    gas_concentration: float
    velocity: float


class TpayloadField(TypedDict):
    team_id: str
    packet_count: int
    altitude: float
    pressure: float
    temperature: float
    orientation_x: float
    orientation_y: float
    orientation_z: float
    accel_x: float
    accel_y: float
    accel_z: float
    gas_concentration: float
    velocity: float


rocket_telemetry: TrocketField
payload_telemetry: TpayloadField


def read_rocket_telemetry():
    global rocket_telemetry
    try:
        serialReadData = rocket_serial.readline().decode().removesuffix("\r\n")
        string_telemetry: dict[str, str] = {}

        (
            string_telemetry["team_id"],
            string_telemetry["time_stamp"],
            string_telemetry["packet_count"],
            string_telemetry["altitude"],
            string_telemetry["pressure"],
            string_telemetry["temperature"],
            string_telemetry["voltage"],
            string_telemetry["gnss_time"],
            string_telemetry["gnss_latitude"],
            string_telemetry["gnss_longitude"],
            string_telemetry["gnss_altitude"],
            string_telemetry["gnss_sats"],
            string_telemetry["orientation_x"],
            string_telemetry["orientation_y"],
            string_telemetry["orientation_z"],
            string_telemetry["accel_x"],
            string_telemetry["accel_y"],
            string_telemetry["accel_z"],
            string_telemetry["software_state"],
            string_telemetry["gas_concentration"],
            string_telemetry["velocity"],
        ) = serialReadData.split(",")

        rocket_telemetry["team_id"] = string_telemetry["team_id"]
        rocket_telemetry["time_stamp"] = string_telemetry["time_stamp"]
        rocket_telemetry["packet_count"] = int(string_telemetry["packet_count"])
        rocket_telemetry["altitude"] = float(string_telemetry["altitude"])
        rocket_telemetry["pressure"] = float(string_telemetry["pressure"])
        rocket_telemetry["temperature"] = float(string_telemetry["temperature"])
        rocket_telemetry["voltage"] = float(string_telemetry["voltage"])
        rocket_telemetry["gnss_time"] = string_telemetry["gnss_time"]
        rocket_telemetry["gnss_latitude"] = float(string_telemetry["gnss_latitude"])
        rocket_telemetry["gnss_longitude"] = float(string_telemetry["gnss_longitude"])
        rocket_telemetry["gnss_altitude"] = float(string_telemetry["gnss_altitude"])
        rocket_telemetry["gnss_sats"] = int(string_telemetry["gnss_sats"])
        rocket_telemetry["orientation_x"] = float(string_telemetry["orientation_x"])
        rocket_telemetry["orientation_y"] = float(string_telemetry["orientation_y"])
        rocket_telemetry["orientation_z"] = float(string_telemetry["orientation_z"])
        rocket_telemetry["accel_x"] = float(string_telemetry["accel_x"])
        rocket_telemetry["accel_y"] = float(string_telemetry["accel_y"])
        rocket_telemetry["accel_z"] = float(string_telemetry["accel_z"])
        rocket_telemetry["software_state"] = string_telemetry["software_state"]
        rocket_telemetry["gas_concentration"] = float(
            string_telemetry["gas_concentration"]
        )
        rocket_telemetry["velocity"] = float(string_telemetry["velocity"])
    except Exception as err:
        print("Could not read Serial Data")
        print(err)

        rocket_telemetry = {
            "team_id": "",
            "time_stamp": "",
            "packet_count": 0,
            "altitude": 0.0,
            "pressure": 0.0,
            "temperature": 0.0,
            "voltage": 0.0,
            "gnss_time": "",
            "gnss_latitude": 0.0,
            "gnss_longitude": 0.0,
            "gnss_altitude": 0.0,
            "gnss_sats": 0,
            "orientation_x": 0.0,
            "orientation_y": 0.0,
            "orientation_z": 0.0,
            "accel_x": 0.0,
            "accel_y": 0.0,
            "accel_z": 0.0,
            "software_state": "",
            "gas_concentration": 0.0,
            "velocity": 0.0,
        }
    finally:
        return rocket_telemetry


def read_payload_telemetry():
    try:
        serialReadData = payload_serial.readline().decode().removesuffix("\r\n")
        string_telemetry = {}

        (
            string_telemetry["team_id"],
            string_telemetry["time_stamp"],
            string_telemetry["packet_count"],
            string_telemetry["altitude"],
            string_telemetry["pressure"],
            string_telemetry["temperature"],
            string_telemetry["voltage"],
            string_telemetry["gnss_time"],
            string_telemetry["gnss_latitude"],
            string_telemetry["gnss_longitude"],
            string_telemetry["gnss_altitude"],
            string_telemetry["gnss_sats"],
            string_telemetry["orientation_x"],
            string_telemetry["orientation_y"],
            string_telemetry["orientation_z"],
            string_telemetry["accel_x"],
            string_telemetry["accel_y"],
            string_telemetry["accel_z"],
            string_telemetry["software_state"],
            string_telemetry["gas_concentration"],
            string_telemetry["velocity"],
        ) = serialReadData.split(",")

        global payload_telemetry
        payload_telemetry["team_id"] = string_telemetry["team_id"]
        payload_telemetry["packet_count"] = int(string_telemetry["packet_count"])
        payload_telemetry["altitude"] = float(string_telemetry["altitude"])
        payload_telemetry["pressure"] = float(string_telemetry["pressure"])
        payload_telemetry["temperature"] = float(string_telemetry["temperature"])
        payload_telemetry["orientation_x"] = float(string_telemetry["orientation_x"])
        payload_telemetry["orientation_y"] = float(string_telemetry["orientation_y"])
        payload_telemetry["orientation_z"] = float(string_telemetry["orientation_z"])
        payload_telemetry["accel_x"] = float(string_telemetry["accel_x"])
        payload_telemetry["accel_y"] = float(string_telemetry["accel_y"])
        payload_telemetry["accel_z"] = float(string_telemetry["accel_z"])
        payload_telemetry["gas_concentration"] = float(
            string_telemetry["gas_concentration"]
        )
        payload_telemetry["velocity"] = float(string_telemetry["velocity"])
    except Exception as err:
        print("Could not read Serial Data")
        print(err)
    finally:
        return payload_telemetry


class TpartialStatus(TypedDict):
    status: bool
    message: str


class Tstatus(TypedDict):
    status: bool
    rocket: bool
    payload: bool
    message: str


def send_rocket_command(sim_mode: bool, command_str: str) -> TpartialStatus:
    if sim_mode:
        if command_str == "calibrate" or "telestart" or "telestop":
            return {
                "status": True,
                "message": f"{command_str} executed successfully for rocket",
            }
        else:
            return {"status": False, "message": "Invalid Command for rocket"}

    try:
        if command_str == "calibrate" or "telestop" or "telestart":
            rocket_serial.write(command_str.encode())
        else:
            raise ValueError("Invalid Command")
    except Exception as err:
        print(err)
        return {"status": False, "message": str(err)}

    res_str: list[str] = (
        rocket_serial.readline().decode().removesuffix("\r\n").split(",")
    )
    res: TpartialStatus = {"status": res_str[0] == "true", "message": str(res_str[1])}

    return res


def send_payload_command(sim_mode: bool, command_str: str) -> TpartialStatus:
    try:
        if command_str == "calibrate" or "telestop" or "telestart":
            payload_serial.write(command_str.encode())
        else:
            raise ValueError("Invalid Command")
    except Exception as err:
        print(err)
        return {"status": False, "message": str(err)}

    res_str: list[str] = (
        payload_serial.readline().decode().removesuffix("\r\n").split(",")
    )
    res: TpartialStatus = {"status": res_str[0] == "true", "message": str(res_str[1])}

    return res
