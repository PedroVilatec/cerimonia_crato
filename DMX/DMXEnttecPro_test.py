from DMXEnttecPro import Controller
from DMXEnttecPro.utils import get_port_by_serial_number, get_port_by_product_id
my_port = get_port_by_serial_number('A10KC0IZ')
dmx = Controller(my_port, dmx_size=256)
dmx.set_dmx_parameters(output_rate=0)
dmx.set_channel(1, 255)
dmx.set_channel(3, 0)
dmx.submit()
