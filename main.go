package main

import (
	"fmt"
	"net"
	"os"
	"sync"
	"time"
	argparse "github.com/akamensky/argparse"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/sirupsen/logrus"
	"gonum.org/v1/gonum/mat"
)
var log = logrus.New()

type Client struct {
	client mqtt.Client
}

func OnConnectHandler(client mqtt.Client) {
	log.Info("Mqtt connected")
}

func ConnectionLostHandler(client mqtt.Client, err error) {
	log.Error(err)
}

func MessageHandler(client mqtt.Client, msg mqtt.Message) {
	log.Info(fmt.Sprintf("Recieve %s from [%s]",msg.Topic(),msg.Payload()))
}

func Mqtt_Server() *Client {
	opts := mqtt.NewClientOptions()
	opts.AddBroker(fmt.Sprintf("tcp://%s:%d", "127.0.0.1", 1883))
	opts.SetDefaultPublishHandler(MessageHandler)
	opts.OnConnect = OnConnectHandler
	opts.OnConnectionLost = ConnectionLostHandler
	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Error(token.Error())
	}
	return &Client{client: client}
}

func (client *Client) Sub(topic string) {
	token := client.client.Subscribe(topic, 0, nil)
	token.Wait()
	log.Info("Mqtt subscribed to topic:", topic)
}

func (client *Client) Unsub(topic string) {
	token := client.client.Unsubscribe(topic)
	token.Wait()
	log.Info("Mqtt unsubscribed to topic:", topic)
}

func (client *Client) Publish(topic string, data []byte) {
	token := client.client.Publish(topic, 0, false, data)
	token.Wait()
}

func loginit() {
	log.SetFormatter(&logrus.TextFormatter{ForceColors:true,TimestampFormat: "2006-01-02 15:04:05.002",FullTimestamp:true})

	log.SetOutput(os.Stdout)
  
	log.SetLevel(logrus.DebugLevel)
  }

func Process(conn net.Conn,n *sync.WaitGroup) string{
	defer conn.Close() 
	defer n.Done()

	var buf []byte = make([]byte,128) 
	for {
		conn.SetReadDeadline(time.Now().Add(time.Second*2))
		n,err := conn.Read(buf)
		if err != nil {
			log.Error(err)
			break
		}
		log.Info("Recieve msg:",string(buf[:n]))
		break
	}
	return string(buf[:len(buf)])
}

func ConnectClient(addr string){
	var address = ":" + addr
	for{
		conn, err := net.Dial("tcp", address)
		if err != nil {
			log.Error(err)
			continue
		}
		conn.Write([]byte("hello world!"))
		log.Info("Send msg:hello world!")
		break 
	}
	
}

func ListenServer(addr string,n *sync.WaitGroup)  {
	var address = ":" + addr
	listen, err := net.Listen("tcp", address)
	if err != nil {
		log.Error(err)
		return
	}
	log.Info("listen to","address")
	conn, err := listen.Accept()
	if err != nil {
		log.Error("Tcp Accept() failed, err: ", err)
		return	
	}
	go Process(conn,n )
}

func Parser() string{
	parser:= argparse.NewParser("", "")
	addr := parser.String("","addr", &argparse.Options{Default: "31000"})
	err := parser.Parse(os.Args)
	if err!=nil{
		return ""
	}
	return *addr
} 
func userVectors() {
	a := mat.NewDense(2, 2, []float64{1, 0,1, 0,})
	fmt.Printf("a = %v\n\n", mat.Formatted(a, mat.Prefix("    "), mat.Squeeze()))
	b := mat.NewDense(2, 2, []float64{0, 1,0, 1,})
	fmt.Printf("b = %v\n\n", mat.Formatted(b, mat.Prefix("    "), mat.Squeeze()))
	var c mat.Dense
	c.Add(a, b)
	fc := mat.Formatted(&c, mat.Prefix("       "), mat.Squeeze())
	fmt.Printf("c=a+b= %v\n", fc)

}
var myclient *Client

func test_mqtt(n *sync.WaitGroup){
	defer n.Done()
	for i:=0;i<10;i++{
		myclient.Publish("test",[]byte("hello mqtt"))
	}
}



func main(){
	loginit()
	userVectors()
	s1:=&Person{Id:0,Name:"j"}
	log.Info("s1.id:",s1.Id,"s1.name:",s1.Name)
	var go_sync sync.WaitGroup
	addr:=Parser()
	myclient=Mqtt_Server()
	myclient.Sub("test")
	go_sync.Add(2)
	go test_mqtt(&go_sync)
	go ConnectClient(addr)
	go ListenServer(addr,&go_sync)
	

	go_sync.Wait()
}
