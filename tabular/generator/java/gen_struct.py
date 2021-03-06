# Copyright (C) 2018-present ichenq@outlook.com. All rights reserved.
# Distributed under the terms and conditions of the Apache License.
# See accompanying files LICENSE.

import os
import sys
import tabular.predef as predef
import tabular.lang as lang
import tabular.version as version
import tabular.util.strutil as strutil
import tabular.util.structutil as structutil
import tabular.generator.java.template as java_template
from tabular.generator.java.gen_csv_load import JavaCsvLoadGenerator


# java代码生成器
class JavaStructGenerator:
    TAB_SPACE = '    '

    @staticmethod
    def name():
        return "java"

    def __init__(self):
        self.load_gen = None

    def setup(self, name, gen_csv_dataload):
        if name is not None:
            if name == 'csv':
                self.load_gen = JavaCsvLoadGenerator(gen_csv_dataload)
            else:
                print('content loader of name %s not implemented' % name)
                sys.exit(1)

    # 合并嵌套类
    @staticmethod
    def gen_java_inner_class(struct):
        content = ''
        class_name = struct["options"][predef.PredefInnerTypeClass]
        inner_fields = structutil.get_inner_class_struct_fields(struct)
        content += '    public static class %s \n' % class_name
        content += '    {\n'
        max_name_len = strutil.max_field_length(inner_fields, 'name', None)
        max_type_len = strutil.max_field_length(inner_fields, 'original_type_name', lang.map_java_type)
        for field in inner_fields:
            typename = lang.map_java_type(field['original_type_name'])
            assert typename != "", field['original_type_name']
            typename = strutil.pad_spaces(typename, max_type_len + 1)
            name = lang.name_with_default_java_value(field, typename)
            name = strutil.pad_spaces(name, max_name_len + 8)
            content += '        public %s %s // %s\n' % (typename.strip(), name, field['comment'])
        content += '    }\n\n'
        return content

    # 生成java类型
    def gen_java_class(self, struct):
        content = ''

        fields = struct['fields']
        if struct['options'][predef.PredefParseKVMode]:
            fields = structutil.get_struct_kv_fields(struct)

        content += '// %s, %s\n' % (struct['comment'], struct['file'])
        content += 'public class %s\n{\n' % struct['name']

        inner_class_done = False
        inner_typename = ''
        inner_var_name = ''
        inner_type_class = ''
        inner_field_names, inner_fields = structutil.get_inner_class_mapped_fields(struct)
        if len(inner_fields) > 0:
            content += self.gen_java_inner_class(struct)
            inner_type_class = struct["options"][predef.PredefInnerTypeClass]
            inner_var_name = struct["options"][predef.PredefInnerTypeName]

        vec_done = False
        vec_names, vec_name = structutil.get_vec_field_range(struct)

        max_name_len = strutil.max_field_length(fields, 'name', None)
        max_type_len = strutil.max_field_length(fields, 'original_type_name', lang.map_java_type)
        if len(inner_typename) > max_type_len:
            max_type_len = len(inner_typename)

        for field in fields:
            if not field['enable']:
                continue
            text = ''
            field_name = field['name']
            if field_name in inner_field_names:
                if not inner_class_done:
                    typename = "ArrayList<>();"
                    text += '    public List<%s> %s = new %s \n' % (inner_type_class, inner_var_name, typename)
                    inner_class_done = True
            else:
                typename = lang.map_java_type(field['original_type_name'])
                assert typename != "", field['original_type_name']
                typename = strutil.pad_spaces(typename, max_type_len + 1)
                if field['name'] not in vec_names:
                    name = lang.name_with_default_java_value(field, typename)
                    name = strutil.pad_spaces(name, max_name_len + 8)
                    text += '    public %s %s // %s\n' % (typename, name, field['comment'])
                elif not vec_done:
                    name = '%s = new %s[%d];' % (vec_name, typename.strip(), len(vec_names))
                    name = strutil.pad_spaces(name, max_name_len + 8)
                    text += '    public %s[] %s // %s\n' % (typename.strip(), name, field['comment'])
                    vec_done = True
            content += text

        return content

    # 生成对象及方法
    def generate_class(self, struct, args):
        content = '\n'
        content += self.gen_java_class(struct)
        if self.load_gen is not None:
            if self.load_gen.gen_csv_dataload:
                content += self.load_gen.gen_static_data(struct)
            else:
                content += '\n'
            content += self.load_gen.gen_parse_method(struct)
            if self.load_gen.gen_csv_dataload:
                content += self.load_gen.gen_load_method(struct)
                content += self.load_gen.gen_get_method(struct)
                content += self.load_gen.gen_range_method(struct)
        content += '}\n'
        return content

    # 生成代码
    def run(self, descriptors, filepath, args):
        mgr_content = '// This file is auto-generated by taxi v%s, DO NOT EDIT!\n\n' % version.VER_STRING
        mgr_filename = '%s.java' % args.config_manager_class

        try:
            os.mkdir(filepath)
        except OSError as e:
            pass

        if args.package is not None:
            pkgname = args.package
            names = [filepath] + pkgname.split('.')
            basedir = '/'.join(names)
            filepath = basedir
            mgr_content += 'package %s;' % pkgname
            mgr_filename = '%s/%s' % (basedir, mgr_filename)
            try:
                print('make dir', basedir)
                os.makedirs(basedir)
            except OSError as e:
                pass

        if self.load_gen:
            (array_delim, map_delims) = strutil.to_sep_delimiters(args.array_delim, args.map_delims)
            self.load_gen.setup(array_delim, map_delims, args.config_manager_class)
            sep_delim = strutil.escape_delimiter(args.out_csv_delim)
            quote_delim = strutil.escape_delimiter('"')
            mgr_content += java_template.JAVA_MGR_CLASS_TEMPLATE % (args.config_manager_class, sep_delim, quote_delim,
                                                                    array_delim, map_delims[0], map_delims[1])

        class_dict = {}

        pkg_imports = [
            'import java.util.*;',
        ]

        if self.load_gen is not None:
            csv_imports = [
                'import java.io.IOException;',
                'import org.apache.commons.csv.*;',
            ]
            pkg_imports += csv_imports

        load_call_content = ''
        for struct in descriptors:
            content = '// This file is auto-generated by Tabular v%s, DO NOT EDIT!\n\n' % version.VER_STRING
            filename = '%s.java' % struct['camel_case_name']
            # print(filename)
            if args.package:
                filename = '%s/%s' % (filepath, filename)
                content += 'package %s;\n\n' % args.package
            content += '\n'.join(pkg_imports)
            content += '\n'
            content += self.generate_class(struct, args)
            class_dict[filename] = content

            if self.load_gen and self.load_gen.gen_csv_dataload:
                name = strutil.camel_to_snake(struct['name'])
                load_call_content += '        %s.loadFrom(reader.apply("%s.csv"));\n' % (struct['name'], name)

        if self.load_gen and self.load_gen.gen_csv_dataload:
            mgr_content += java_template.JAVA_LOAD_ALL_METHOD_TEMPLATE % load_call_content
        mgr_content += '}\n'

        if self.load_gen is not None:
            class_dict[mgr_filename] = mgr_content

        for filename in class_dict:
            content = class_dict[filename]
            filename = os.path.abspath(filename)
            strutil.save_content_if_not_same(filename, content, args.source_file_encoding)
            print('wrote Java source file to', filename)
