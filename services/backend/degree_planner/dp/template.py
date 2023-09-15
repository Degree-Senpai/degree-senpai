from ..math.attributes import Attributes

class Template():

    def __init__(self, name):
        self.name = name
        self.requirements = list()
        self.groups = list()
        self.properties = dict()
        self.attributes = Attributes(False)
        self.group_attributes = Attributes(False, '\\')

    def __str__(self):
        return self.name
    
    def __repr__(self):
        return f"template {self.name} with requirements: {[repr(e) for e in self.requirements]} \ngroups: {self.group_attributes} \nproperties: {self.properties}\n\n"
    
    def __eq__(self, other):
        if not isinstance(other, Template):
            return False
        return self.name == other.name
    
    def __hash__(self):
        return hash(self.name)
