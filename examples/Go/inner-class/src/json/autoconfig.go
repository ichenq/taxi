// This file is auto-generated by Tabular v1.4.0, DO NOT EDIT!
package config

import (
	"bytes"
	"encoding/csv"
	"io"
	"log"
	"strings"
)

var (
	_ = io.EOF
	_ = strings.Split
	_ = log.Panicf
	_ = bytes.NewReader
	_ = csv.NewReader
)

type ProbabilityGoodsDefine struct {
	GoodsID     string `json:"goods_id"`    // 物品1id
	Num         uint32 `json:"num"`         // 物品1数量
	Probability uint32 `json:"probability"` // 物品1概率
}

// , 随机宝箱.xlsx
type BoxProbabilityDefine struct {
	ID               string                   `json:"id"`                // ID
	Total            int                      `json:"total"`             // 奖励总数
	Time             int                      `json:"time"`              // 冷却时间
	Repeat           bool                     `json:"repeat"`            // 是否可重复
	ProbabilityGoods []ProbabilityGoodsDefine `json:"probability_goods"` //
}
