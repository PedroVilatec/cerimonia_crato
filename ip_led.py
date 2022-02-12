import threading
import time
import config
class Leds:
    def __init__(self):
        self.wait = 0
        self.LedsWifi = config.CONTROLADORES
        self.inicio = time.time() + 20
        self.send_telegram_single = "" # variavel sentenciada para enviar telegramas
        self.send_telegram_all = "" # variavel sentenciada para enviar telegramas
        thread1 = threading.Thread(target=self.verifica_ip)
        thread1.start()
    def getIp(self, device):
        '''
        Busca o ip do controlador led pelo nome
        '''
        for k, v in self.LedsWifi.items():
            #print(k)
            if k == device:
                return self.LedsWifi[k]['IP']

    def verifica_ip(self):
        mac_list = []
        for k, v in self.LedsWifi.items():
            mac_list.append(self.LedsWifi[k]['MAC'])
            self.LedsWifi[k]['COUNTER'] = 0 # CONTADOR PARA VERIFICAR LED OnLINE  
            self.LedsWifi[k]['MSG_ENVIADA'] = False # CONTADOR PARA VERIFICAR LED ODDLINE  

        while True:
            cmd="flux_led -s"
            p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
            bulbs = p.stdout.readlines()
            del bulbs[0]
            if len(self.LedsWifi) == len(bulbs):
                self.wait = 5
        
            for itens in bulbs:
                #print(itens)
                itens = itens.decode().rstrip()
                if "  " in itens:
                    itens = itens.replace("  ","")
                endereco_mac = itens.split(" ")[0]
                endereco_ip = itens.split(" ")[1]    
                for k, v in self.LedsWifi.items():
                        
                    if self.LedsWifi[k]['MAC'] in endereco_mac:
                        self.LedsWifi[k]['IP'] = endereco_ip
                        self.LedsWifi[k]['COUNTER'] = 0
                        
                        if have_internet() == "Conectado" and self.LedsWifi[k]['MSG_ENVIADA'] == True:
                            telegram_bot.envia_telegram_all("{}: controlador wifi {} online novamente.".format(config.CAPELA, k))                        
                            self.LedsWifi[k]['MSG_ENVIADA'] = False                            
                            
                    if self.LedsWifi[k]['COUNTER'] >= config.TENTATIVAS_LED:
                        
                        if have_internet() == "Conectado" and self.LedsWifi[k]['MSG_ENVIADA'] == False:
                            telegram_bot.envia_telegram_all("{}: controlador wifi {} offline.".format(config.CAPELA, k))                     
                            self.LedsWifi[k]['MSG_ENVIADA'] = True
                            self.LedsWifi[k]['COUNTER'] = 0
                    else:
                        self.LedsWifi[k]['COUNTER'] += 1
                        
                    if not endereco_mac in mac_list:
                        print("Novo Controlador encontrado", endereco_mac)
                
                       
                
                        
                    # ~ else:
                        # ~ print("Novo controlador encontrado", endereco_mac, endereco_ip)
            time.sleep(self.wait)
