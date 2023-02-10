package main

import (
	"testing"
	"google.golang.org/protobuf/proto"
	"sync"
	"time"
)

func TestProto(t *testing.T) {
	before:=&Person{Id:0,Name:"j"}
	buffer, _ := proto.Marshal(before)
	after:=&Person{}
	proto.Unmarshal(buffer,after)
	if before.Id==after.Id{
		t.Logf("person.Id 执行正确...")
	}else{
		t.Fatalf("person.Id 执行错误!before.Id是=%v after.Id=%v\n",before.Id,after.Id)
	}
	if before.Name==after.Name{
		t.Logf("person.name 执行正确...")
	}else{
		t.Fatalf("person.Name 执行错误!before.Name=%v after.Name=%v\n",before.Name,after.Name)
	}
}

func TestMQTT(t *testing.T) {
	var client=Mqtt_Server()
	client.Sub("testing")
	for i:=0;i<10;i++{
		client.Publish("testing",[]byte("hello mqtt"))
	}

}

func TestVectors(t *testing.T) {
	userVectors()
}

func TestNet(t *testing.T) {
	var go_sync sync.WaitGroup
	go ListenServer("12345",&go_sync)
	time.After(time.Second*2)
	go ConnectClient("12345",&go_sync)
	go_sync.Wait()

}