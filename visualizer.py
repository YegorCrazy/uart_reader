import json
import re
import socket

def parseCoordinates(str: str):
    longMatch = re.search('(\d{4}\.\d+)\,([NS])', str)
    if (longMatch == None):
        return None
    try:
        longValue = int(longMatch[1][0:2]) + float(longMatch[1][2:]) / 60
    except ValueError:
        return None
    if (longMatch[2] == 'S'):
        longValue *= -1
    
    lattMatch = re.search('(\d{4}\.\d+)\,([EW])', str)
    if (lattMatch == None):
        return None
    try:
        lattValue = int(lattMatch[1][0:2]) + float(lattMatch[1][2:]) / 60
    except ValueError:
        return None
    if (lattMatch[2] == 'W'):
        lattValue *= -1

    return [lattValue, longValue]


if __name__ == "__main__":

    UDP_IP = '127.0.0.1'
    UDP_PORT = 5678

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    geoJsonIoJson = (
        {
            "type": "FeatureCollection",
            "features": 
            [
                {
                    "type": "Feature",
                    "properties": {},
                    "geometry": 
                    {
                        "coordinates": [],
                        "type": "LineString"
                    }
                }
            ]
        }
    )

    while (True):
        try:
            data = sock.recv(1024)
            if (data[0] == 0xff):
                data = data[1:]
            try:
                parseResult = parseCoordinates(data.decode())
            except UnicodeError:
                print('undecodable string found')
                continue
            if (parseResult == None):
                print('unparcable string found')
            else:
                geoJsonIoJson['features'][0]['geometry']['coordinates'].append(parseResult)
        except KeyboardInterrupt:
            print(json.JSONEncoder().encode(geoJsonIoJson))