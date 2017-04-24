import pytid

client = pytid.PyTiDClient()
client.connect('127.0.0.1',9001)

while True:
    msg = client.wait4NewTiDMessage()
    print(msg.GetEvent())
    print(msg.GetFamily())
    msg.Dump()
