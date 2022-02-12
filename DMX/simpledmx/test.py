import dmx
import time
mydmx = dmx.DMXConnection(6)
mydmx.setChannel(3, 0, autorender=True)
time.sleep(2)
mydmx.setChannel(2, 0, autorender=True)
time.sleep(2)
mydmx.setChannel(500, 0, autorender=True)
