##restful_get1
package main

import (
	"fmt"
	"net/http"
	"io/ioutil"
)

func main() {
	url := "http://127.0.0.1:9980/registry/v3/microservices/1"
	ret, err := http.Get(url)
	str := ret.Header.Get("X-ConsumerId")

	fmt.Println("str : ", str)

	if err != nil {
		panic(err)
	}
	defer ret.Body.Close()

	fmt.Println("ret : ", ret)
	fmt.Println("ret : ", ret.Body)
	body, err := ioutil.ReadAll(ret.Body)
	if err != nil {
		panic(err)
	}
	fmt.Printf("body : %s", body)
}


##restful_get2
package main

import (
	"fmt"
	"net/http"
	"io/ioutil"
)

func main() {
	client := &http.Client{}
	url := "http://127.0.0.1:9980/registry/v3/microservices/1/instances"
	ret, err := http.NewRequest("GET", url, nil)
	ret.Header.Add("X-ConsumerId", "1")

	response, _ := client.Do(ret)

	if err != nil {
		panic(err)
	}

	fmt.Println("ret : ", ret)
	fmt.Println("ret.Body : ", ret.Body)
	fmt.Println("response : ", response)
	fmt.Println("response.Body : ", response.Body)

	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		panic(err)
	}
	fmt.Printf("body : %s", body)
}


##restful_post
package main

import "fmt"
import (
	"net/http"
	"reflect"
	"io/ioutil"
	"strings"
)

func main() {
	jsonstr := `{"instance":{"endpoints": ["grpc:127.0.0.1:9980","rest:127.0.0.1:8080"],"hostName":"szx1234","status":"UP","properties": {"attr1": "a"}}}`

	client := &http.Client{}
	url := "http://127.0.0.1:9980/registry/v3/microservices/1/instances"
	fmt.Printf("jsonstr's type : %s\n", reflect.TypeOf(jsonstr))
	ret, err := http.NewRequest("POST", url, strings.NewReader(jsonstr))

	response, _ := client.Do(ret)

	if err != nil {
		panic(err)
	}

	fmt.Println("response : ", response)
	fmt.Println("response.Body : ", response.Body)

	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		panic(err)
	}
	fmt.Printf("body : %s\n", body)
	fmt.Printf("body's type : %s\n", reflect.TypeOf(body))
}


##websocket
package main

import (
	"golang.org/x/net/websocket"
	"fmt"
	"log"
)

var origin = "http://127.0.0.1:9980"
var url = "ws://127.0.0.1:9980/registry/v3/microservices/1/watcher"

func main() {

	ws, err := websocket.Dial(url, "", origin)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("Web Socket is connected.\n")
	var msg = make([]byte, 4096)
	//var msg []byte
	var n int
	for {
		if n, err = ws.Read(msg); err != nil {
			log.Fatal(err)
		}
		fmt.Printf("Received: %s.\n", msg[:n])
	}
}
