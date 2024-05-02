import serial
import serial.tools.list_ports
import inquirer

RPM_CONSTANT = 0.1885

ser: serial.Serial = None

def connect():
  global ser
  # Open the serial port
  ports = serial.tools.list_ports.grep("CP2102")

  choices = []
  choice = None

  for port in ports:
    name = f"{port.device} {port.description}"
    choices.append((name, port.device))

  if len(choices) == 0:
    print("No ports found")
    exit(1)
    
  if len(choices) == 1:
    choice = choices[0][1]
  else:
    answer = inquirer.prompt([
      inquirer.List('port',
                    message="Select the port",
                    choices=choices,
                ),
    ])
    choice = answer['port']

  print(f"Opening port {choice}")

  ser = serial.Serial(choice, 9600)
  ser.close()
  ser.open()
  print("Port opened")

connect()

if ser is None:
  print("No serial port found")
  exit(1)

buffer = bytearray([0x00] * 12)
buffer[0] = 0x41
buffer[1] = 0
buffer[2] = 0x00
buffer[3] = 0
buffer[4] = 0
buffer[5] = 0
buffer[6] = 0
buffer[7] = 0
buffer[8] = 0
buffer[9] = 0
buffer[10] = 0x00
buffer[11] = 0x00

def calculate_checksum():
  global buffer
  checksum = 0
  for i in range(1, 12):
    if i == 6:
      continue
    
    checksum ^= buffer[i]
    
  return checksum

def set_option(index: int, value: int):
  global buffer
  buffer[index] = value
  buffer[6] = calculate_checksum()

while True:
  action = inquirer.prompt([
    inquirer.List('action',
                  message="Select value value",
                  choices=[
                    ("Speed", 'speed'),
                    ("Brake", 'brake'),
                    ("Power", 'power'),
                    ("Temperature", 'temperature'),
                    ("Exit", 'exit'),
                  ],
              ),
  ])['action']
  
  if action == 'exit':
    break
  
  value = inquirer.prompt([
    inquirer.Text('value',
                  message="Enter the value",
              ),
  ])['value']
  
  if action == 'speed':
    speed_bytes = round(60000 / (int(value) / 0.66 / RPM_CONSTANT))
    speed_byte_1 = speed_bytes >> 8
    speed_byte_2 = speed_bytes & 0xFF
    
    set_option(3, speed_byte_1)
    set_option(4, speed_byte_2)
  elif action == 'brake':
    if int(value) == 0:
      set_option(7, 0)
    else:
      set_option(7, 32)
  elif action == 'power':
    val = int(int(value) / 13)
    
    set_option(8, val)
    
  ser.write(buffer)
  
ser.close()