package main

import (
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
)

func main() {
	data := make(map[string]bool)
	on_off_text := map[bool]string{true: "ON", false: "OFF"}

	engine := gin.Default()
	engine.LoadHTMLGlob("src/*")

	engine.GET("/", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", gin.H{})
	})

	engine.GET("/list", func(c *gin.Context) {
		text := ""
		for k, v := range data {
			text += fmt.Sprintf("%s:%s\n", k, on_off_text[v])
		}
		c.String(http.StatusOK, text)
	})

	engine.GET("/data/:id", func(c *gin.Context) {
		id := c.Param("id")
		c.String(http.StatusOK, on_off_text[data[id]])
	})

	engine.POST("/data", func(c *gin.Context) {
		type Json struct {
			Id    string `json:"id"`
			Check bool   `json:"check"`
		}
		var json Json
		c.BindJSON(&json)
		data[json.Id] = json.Check
	})
	engine.Run(":80")
}
