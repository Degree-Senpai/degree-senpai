from enum import Enum
from .template import Template
from .requirement import Requirement
from .catalog import Catalog
from ..io.output import Output
from ..math.array_math import array_functions as af
from ..math.attributes import Attributes

DPT_PATH = Output.DATA_FOLDER_PATH + "templates.dpt"
INDENT_SPACES = 4
DIR_DELIMITER = '\\'

class Scope(Enum):
    template=1
    requirement=2
    group=3
    empty=10

class Parser():
    @staticmethod
    def import_dpt(catalog:Catalog, path=None):
        if path is None:
            path = DPT_PATH
        Parser.parse_file(Parser.open_file(path), catalog)

    @staticmethod
    def open_file(path):
        with open(path) as f:
            lines = f.read().splitlines()
            return lines

    @staticmethod
    def parse_file(file, catalog:Catalog):
        scope = []
        scope_val = []
        template_obj = None
        requirement_obj = None

        for line in file:
            line:str
            if '//' in line:
                line = line[:line.find('//')]
            if line.strip() == '':
                continue
            tabs, line = line_tabs(line)
            stretch_scope(scope, scope_val, tabs)
            if tabs < len(scope):
                scope = scope[:tabs]
                scope_val = scope_val[:tabs]

            print(f'scope: {scope}, val: {scope_val}')

            if line.startswith('template '):
                scope.append(Scope.template)
                begin_quote = line.find('"') + 1
                end_quote = line.find('"', begin_quote)
                name = line[begin_quote:end_quote]
                scope_val.append(name)

                template = Template(name)
                template_obj = template
                catalog.add(template)

            elif line.startswith('requirement '):
                scope.append(Scope.requirement)
                begin_quote = line.find('"') + 1
                end_quote = line.find('"', begin_quote)
                name = line[begin_quote:end_quote]
                print(f'NAME :: {name}')
                scope_val.append(name)

                requirement = Requirement(name)
                requirement_obj = requirement
                template_obj.group_attributes.add_attribute(f"{get_group_string(scope, scope_val)}{DIR_DELIMITER}{name.casefold()}")
                template_obj.requirements.append(requirement)

            elif line.startswith('group '):
                scope.append(Scope.group)
                begin_quote = line.find('"') + 1
                end_quote = line.find('"', begin_quote)
                name = line[begin_quote:end_quote]
                scope_val.append(name)

            elif len(scope) and scope[-1] == Scope.template:
                prop_key, prop_val = parse_props(line)
                if prop_key == 'attribute':
                    template_obj.attributes.add_attribute(prop_val)
                template_obj.properties.update({prop_key:prop_val})

            elif len(scope) and scope[-1] == Scope.group:
                prop_key, prop_val = parse_props(line)
                template_obj.group_attributes.add_attribute(f"{get_group_string(scope, scope_val)}{DIR_DELIMITER}{prop_key}={prop_val}")
            
            elif len(scope) and scope[-1] == Scope.requirement:
                prop_key, prop_val = parse_props(line)
                if prop_key == 'specifications':
                    prop_val = parse_specifications(prop_val)
                    print(f'parsed {prop_val}')
                    requirement_obj.add_specification(prop_val)
                    continue
                if prop_key == 'count' and (isinstance(prop_val, int) or (isinstance(prop_val, str) and prop_val.isdigit())):
                    prop_val = int(prop_val)
                    requirement_obj.elements_required = prop_val
                    continue
                requirement_obj.properties.update({prop_key:prop_val})

        print(f"finished parsing dpt! {list(catalog.get_templates())}")

def stretch_scope(scope:list, scope_val:list, length) -> None:
    if length > len(scope):
        for _ in range(len(scope), length):
            scope.append(Scope.empty)
            scope_val.append('')

def get_group_scope(scope:list, scope_val:list):
    groups = []
    for i in range(len(scope)):
        if scope[i] == Scope.group:
            groups.append(scope_val[i])
    return groups

def get_group_string(scope:list, scope_val:list):
    return DIR_DELIMITER.join(get_group_scope(scope, scope_val))

def line_tabs(line:str):
    count = 0
    stripped = line
    for char in line:
        if char == '\t':
            count += 1
        else:
            break
    stripped = line[count:]
    if count == 0:
        for char in line:
            if char == ' ':
                count += 1
            else:
                break
        stripped = line[count:]
        count = int(count / 4)
    return count, stripped

def parse_props(line:str):
    if '=' in line:
        prop_key = line[:line.find('=')].strip()
        prop_val = line[line.find('=') + 1:].strip()
    else:
        return line.strip(), ''
    prop_val = parse_item(prop_val)

    return prop_key, prop_val

def parse_item(s:str):
    if s.startswith('"') and s.endswith('"'):
        s = s.strip('"')
    
    elif parse_number(s) is not None:
        s = parse_number(s)

    elif parse_boolean(s) is not None:
        s = parse_boolean(s)
    
    return s

def parse_number(s:str):
    try:
        return int(s) if s.isdigit() else float(s)
    except ValueError:
        return None
    
def parse_boolean(s:str):
    if s == 'true':
        return True
    elif s == 'false':
        return False
    return None

TERMINATORS = ('$', '|', '&', '~', '(', ')', ' ')

def parse_specifications(s:str):
    begin_index = 0
    while s.find('$', begin_index) != -1:
        begin_index = s.find('$', begin_index) + 1
        end_index = af.find_set(s, TERMINATORS, begin_index)
        if end_index == -1:
            end_index = len(s)
        word = s[begin_index:end_index]

        if len(word) == 8 and word[4:].isdigit():
            replacement = f"(subject.{word[:4]} & course_id.{word[4:]})"
            s = s[:begin_index - 1] + replacement + s[end_index:]
    return s
