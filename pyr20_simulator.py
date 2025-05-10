import time
from pymodbus.server import StartSerialServer
from pymodbus.device import ModbusDeviceIdentification
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
from threading import Thread

# Configuração inicial dos registradores Modbus
ir_values = [0] * 10  # Inicializa os registradores com zero
store = ModbusSlaveContext(
    di=ModbusSequentialDataBlock(0, [0] * 10),  # Discrete Inputs (não utilizado)
    co=ModbusSequentialDataBlock(0, [0] * 10),  # Coils (não utilizado)
    hr=ModbusSequentialDataBlock(0, [0] * 10),  # Holding Registers (não utilizado)
    ir=ModbusSequentialDataBlock(0, ir_values)  # Input Registers (registrador de irradiância solar)
)

# Contexto do servidor Modbus
context = ModbusServerContext(slaves=store, single=True)

# Identificação do dispositivo Modbus
identity = ModbusDeviceIdentification()
identity.VendorName = 'Simulador'
identity.ProductCode = 'PYR20'
identity.ProductName = 'Piranômetro Simulado'
identity.ModelName = 'Modelo RTU'
identity.MajorMinorRevision = '1.0'

# Função para atualizar o valor do registrador de irradiância solar
def update_irradiance():
    global ir_values
    while True:
        for value in range(0, 2001, 1):  # Incrementa de 0 a 2000 em passos de 100
            ir_values[0] = value  # Atualiza o primeiro registrador de entrada
            store.setValues(4, 0, ir_values)  # Escreve no registrador Modbus
            print(f"Irradiância atualizada: {value} W/m²")
            time.sleep(1)

# Inicia a atualização da irradiância em uma thread separada
Thread(target=update_irradiance, daemon=True).start()

# Iniciar o servidor Modbus RTU
print("Servidor Modbus RTU iniciado...")
StartSerialServer(
    context,
    identity=identity,
    port='COM18',       # Porta serial (ajuste conforme necessário)
    baudrate=9600,     # Baud rate padrão do PYR20
    stopbits=1,        # Stop bits padrão do PYR20
    bytesize=8,        # Byte size padrão do PYR20
    parity='N',        # Paridade padrão do PYR20 (None)
    method='rtu'       # Modo RTU
)
