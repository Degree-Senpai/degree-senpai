class Attributes():

    def __init__(self, casefold=True, delimiter='.'):
        self.attributes_head_to_body_str = dict()
        self.attributes_full_str_to_list = dict()
        self.attributes_order = list()

        self.DIR_ITEMS_CATEGORY = "$items"
        self.DIR_ORDER_CATEGORY = "$order"
        self.DELIMITER = delimiter
        self.casefold = casefold
    
    # TODO finish ordered dictectory implementation

    def to_directory_add_item(self, directory, item:list, ordered=False, by_tail=False):
        directory_traversal = directory
        for i in range(len(item) - 1):
            if item[i] not in directory_traversal:
                directory_traversal.update({item[i]:{}})
                if ordered:
                    order = directory_traversal.get(self.DIR_ORDER_CATEGORY, [])
                    order.append(item[i])
                    directory_traversal.update({self.DIR_ORDER_CATEGORY:order})
            elif ordered and by_tail:
                order = directory_traversal.get(self.DIR_ORDER_CATEGORY, [])
                order.remove(item[i])
                order.append(item[i])
                directory_traversal.update({self.DIR_ORDER_CATEGORY:order})
            directory_traversal = directory_traversal[item[i]]
        
        items_list = directory_traversal.get(self.DIR_ITEMS_CATEGORY, [])
        #print(f"directory: {directory} traversal: {directory_traversal} items_list: {items_list} item: {item[-1]}")
        items_list.append(item[-1])
        if not ordered:
            items_list.sort()
        directory_traversal.update({self.DIR_ITEMS_CATEGORY:items_list})
        #print(f"directory after: {directory} traversal: {directory_traversal} items_list: {items_list} item: {item[-1]}")

    def to_directory(self, ordered=False, by_tail=False):
        directory = dict()
        for item in self.attributes_order:
            self.to_directory_add_item(directory, self.attributes_full_str_to_list.get(item), ordered, by_tail)
        return directory

    def add_attribute(self, attr:str):
        if self.casefold:
            attr = attr.casefold()
 
        attr_split = attr.split(self.DELIMITER)
        self.attributes_full_str_to_list.update({attr:attr_split})
        if attr in self.attributes_order:
            self.attributes_order.remove(attr)
        self.attributes_order.append(attr)        
        for i in range(0, len(attr_split) + 1):
            head = self.DELIMITER.join(attr_split[:i])
            body = self.DELIMITER.join(attr_split[i:])
            update_dict(self.attributes_head_to_body_str, head, body)
        # print(f'added attribute {attr}, current states: \n full_str: {self.attributes_full_str_to_list}\n head and body: {self.attributes_head_to_body_str}')

    def remove_attribute(self, attr:str):
        attr_split = attr.split(self.DELIMITER)
        if self.attributes_full_str_to_list.pop(attr, None) is None:
            return
        self.attributes_order.remove(attr)
        for i in range(0, len(attr_split) + 1):
            head = self.DELIMITER.join(attr_split[:i])
            body = self.DELIMITER.join(attr_split[i:])
            update_dict(self.attributes_head_to_body_str, head, body, True)
        # print(f'removed attribute {attr}, current states: \n full_str: {self.attributes_full_str_to_list}\n head and body: {self.attributes_head_to_body_str}')

    def get_attributes_by_head(self, attr:str):
        '''
        returns all attributes with that head
        '''
        if attr == '':
            return list(self.attributes_full_str_to_list.keys())
        bodies = self.attributes_head_to_body_str.get(attr, {})
        attributes = list()
        for body in bodies:
            if body == '':
                attributes.append(f'{attr}')
            else:
                attributes.append(f'{attr}.{body}')
        return attributes
    
    def get_attributes_body_by_head(self, attr:str):
        bodies = self.attributes_head_to_body_str.get(attr, {})
        return list(bodies)

    def remove_attributes_by_head(self, attr:str):
        for attribute in self.get_attributes_by_head(attr):
            self.remove_attribute(attribute)

    def replace_attribute(self, head:str, body:str):
        if len(self.get_attributes_by_head(head)) > 0:
            self.remove_attributes_by_head(head)
            self.add_attribute(f'{head}.{body}')

    def get_attributes_ge(self, attr):
        bodies_of_head_matches = self.get_attributes_body_by_head(self.no_tail(attr))
        bodies_ge = [possibility for possibility in bodies_of_head_matches if possibility >= self.tail(attr)]
        # print(f"attribute {attr} with no tail {no_tail(attr)} matched with bodies {bodies_of_head_matches} with bodies ge {bodies_ge}")
        return [f"{self.no_tail(attr)}.{body}" if self.no_tail(attr) != '' else body for body in bodies_ge]
    
    def get_attributes_le(self, attr):
        bodies_of_head_matches = self.get_attributes_body_by_head(self.no_tail(attr))
        bodies_le = [possibility for possibility in bodies_of_head_matches if possibility <= self.tail(attr)]
        # print(f"attribute {attr} with no tail {no_tail(attr)} matched with bodies {bodies_of_head_matches} with bodies ge {bodies_ge}")
        return [f"{self.no_tail(attr)}.{body}" if self.no_tail(attr) != '' else body for body in bodies_le]

    def attr(self, head:str):
        '''
        returns only the body of the attribute specified
        '''
        bodies = self.attributes_head_to_body_str.get(head, None)
        # print(f'get attribute with head {head}, current states: \n full_str: {self.attributes_full_str_to_list}\n head and body: {self.attributes_head_to_body_str}\n found {bodies}')
        if bodies is None:
            return None
        if len(bodies) == 1:
            for body in bodies:
                return body
        return bodies
    
    def next_attr(self, head:str):
        next_values = list()
        for body in self.attributes_head_to_body_str.get(head):
            if len(body):
                next_values.append(body[:body.find(self.DELIMITER)])
        return next_values

    def __eq__(self, other):
        if not isinstance(other, Attributes):
            return False
        for head in self.attributes_full_str_to_list.keys():
            if head not in other.attributes_full_str_to_list:
                return False
        return True
    
    def __add__(self, other):
        attributes = Attributes(self.casefold)
        for attr in self.attributes_full_str_to_list.keys():
            attributes.add_attribute(attr)
        for attr in other.attributes_full_str_to_list.keys():
            attributes.add_attribute(attr)
        return attributes
    
    def __iter__(self):
        for attr in self.attributes_order:
            yield attr

    def __len__(self):
        return len(self.attributes_full_str_to_list)
    
    def __str__(self):
        return ', '.join(self.attributes_full_str_to_list.keys())
    
    def __repr__(self):
        return ', '.join(self.attributes_full_str_to_list.keys())
    
    def __hash__(self):
        hash = 0
        for attr in self.attributes_full_str_to_list.keys():
            hash += hash(attr)
        return hash


    def head(self, attr:str) -> str:
        return attr[:attr.find(self.DELIMITER)]

    def body(self, attr:str) -> str:
        if self.DELIMITER not in attr:
            return ''
        return attr[attr.find(self.DELIMITER) + 1:]

    def tail(self, attr:str) -> str:
        return attr[attr.rfind(self.DELIMITER) + 1:]

    def no_tail(self, attr:str) -> str:
        if self.DELIMITER not in attr:
            return ''
        return attr[:attr.rfind(self.DELIMITER)]

def update_dict(dictionary:dict, key, value, remove=False):
    if remove:
        dictionary.get(key).discard(value)
        if not len(dictionary.get(key)):
            dictionary.pop(key)
        return

    if key in dictionary:
        dictionary.get(key).add(value)
    else:
        dictionary.update({key:{value}})
