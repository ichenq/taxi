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

const (
	TAB_CSV_SEP     = `,` // CSV field delimiter
	TAB_CSV_QUOTE   = `"` // CSV field quote
	TAB_ARRAY_DELIM = `,` // array item delimiter
	TAB_MAP_DELIM1  = `;` // map item delimiter
	TAB_MAP_DELIM2  = `=` // map key-value delimiter
)

// 新手引导配置, 新手任务.xlsx
type NewbieGuideDefine struct {
	Name           string            // ID
	Type           string            // 任务类型
	Target         string            // 目标
	Accomplishment []int16           // 完成步骤
	Goods          map[string]uint32 // 物品
	Description    string            // 描述
}

func (p *NewbieGuideDefine) ParseFromRow(row []string) error {
	if len(row) < 6 {
		log.Panicf("NewbieGuideDefine: row length too short %d", len(row))
	}
	if row[0] != "" {
		p.Name = row[0]
	}
	if row[1] != "" {
		p.Type = row[1]
	}
	if row[2] != "" {
		p.Target = row[2]
	}
	if row[3] != "" {
		for _, item := range strings.Split(row[3], TAB_ARRAY_DELIM) {
			var value = ParseStringAs("int16", item)
			p.Accomplishment = append(p.Accomplishment, value.(int16))
		}
	}
	if row[4] != "" {
		p.Goods = map[string]uint32{}
		for _, text := range strings.Split(row[4], TAB_MAP_DELIM1) {
			if text == "" {
				continue
			}
			var items = strings.Split(text, TAB_MAP_DELIM2)
			var value = ParseStringAs("string", items[0])
			var key = value.(string)
			value = ParseStringAs("uint32", items[1])
			var val = value.(uint32)
			p.Goods[key] = val
		}
	}
	if row[5] != "" {
		p.Description = row[5]
	}
	return nil
}

func LoadNewbieGuideDefineList(data []byte) ([]*NewbieGuideDefine, error) {
	var list []*NewbieGuideDefine
	var r = csv.NewReader(bytes.NewReader(data))
	for i := 0; ; i++ {
		row, err := r.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Printf("NewbieGuideDefine: read csv %v", err)
			return nil, err
		}
		var item NewbieGuideDefine
		if err := item.ParseFromRow(row); err != nil {
			log.Printf("NewbieGuideDefine: parse row %d, %s, %v", i+1, row, err)
			return nil, err
		}
		list = append(list, &item)
	}
	return list, nil
}
