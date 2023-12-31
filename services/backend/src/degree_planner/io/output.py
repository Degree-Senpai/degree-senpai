'''
Output class
'''

import logging
from enum import Enum
import json
import os
from ..math.dictionary_array import Dict_Array

class Output():
    '''
    Handles printing output to specified location

    Args:
        output_location (OUT): Enum that describes location to print into
        output_type (OUTTYPE): How to format output
        file (file): file to print to if printing to file
        signature (str): used for embed titles
    '''

    class OUT(Enum):
        NONE = 0

        CONSOLE = 1
        INFO = 2
        DEBUG = 3
        WARN = 4
        ERROR = 5

        STORE = 6
        FILE = 7

        VISUALIZE = 8

    class OUTTYPE(Enum):
        STRING = 1
        JSON = 2


    DATA_FOLDER_PATH = os.getcwd() + "/src/degree_planner/data/"
    print(f'data path: {DATA_FOLDER_PATH}')
    visualizers = dict()

    @staticmethod
    def visualize(visualizer_name, *args):
        visualizer = Output.visualizers.get(visualizer_name)
        if visualizer is not None:
            visualizer.visualize(*args)


    def __init__(self, output_location:OUT, output_type:OUTTYPE=OUTTYPE.STRING, user=None, 
            file=None, signature:str=None, auto_clear=False):

        self.output_location = output_location
        self.output_type = output_type
        self.file = file
        self.user = user
        self.auto_clear = auto_clear

        self.cache = list()
        self.signature = signature

        self.DELIMITER_TITLE = ' :: '
        self.LJUSTIFY = 13


    def println(self, printout, output_location:OUT=None, file_name:str=None) -> None:
        self.print('\n' + printout, output_location=output_location, file_name=file_name)


    def print(self, printout, output_location:OUT=None, file_name:str=None, no_signature:bool=False) -> None:
        '''
        Determines appropriate printing channel and prints message

        Args:
            msg (str/dict): message to print
            
            logging_flag (OUT): temporary prints to this output location
                without altering the stored location within this object
        '''

        output_location = self.output_location if output_location == None else output_location

        if output_location == self.OUT.NONE:
            return

        if self.output_type == self.OUTTYPE.JSON:
            if isinstance(printout, dict):
                output = json.dumps(printout)
            elif isinstance(printout, str):
                output = json.dumps({'MESSAGE':printout})
            elif isinstance(printout, json):
                output = printout
        else:
            output = ''
            if isinstance(printout, dict):
                for entry_key, entry_value in printout.items():
                    output += f'{entry_key}{self.DELIMITER_TITLE}{entry_value}\n'
            elif isinstance(printout, str):
                if not no_signature and not self.signature is None and len(self.signature):
                    output = f'{self.signature.ljust(self.LJUSTIFY) if self.LJUSTIFY != 0 else self.signature}{self.DELIMITER_TITLE}{printout}'
                else:
                    output = printout
            elif isinstance(printout, json):
                output = str(json.loads(printout))

            if output_location == self.OUT.INFO:
                logging.info(output)
            elif output_location == self.OUT.DEBUG:
                logging.debug(output)
            elif output_location == self.OUT.WARN:
                logging.warning(output)
            elif output_location == self.OUT.ERROR:
                logging.error(output)
            elif output_location == self.OUT.CONSOLE:
                print(output)

        if (output_location == self.OUT.STORE):
            self.cache.append(output)

        elif (output_location == self.OUT.FILE):
            if file_name is None:
                file_name = self.file
            f = open(file_name, 'a')
            f.write(output)
            f.close


    def store(self, printout):
        self.print(printout, self.OUT.STORE, no_signature=True)

    def view_cache(self, output_redirect=None) -> None:
        '''
        prints cache
        '''
        if output_redirect == None:
            for line in self.cache:
                self.print(line, no_signature=True)
        else:
            for line in self.cache:
                self.print(line, output_location=output_redirect, no_signature=True)
        
        if self.auto_clear:
            self.cache.clear()

    def peek_cache(self, output_redirect=None) -> None:
        '''
        print the most recent entry in cache
        '''
        if not len(self.cache):
            return
        if output_redirect == None:
            self.print(self.cache[-1], no_signature=True)
        else:
            self.print(self.cache[-1], output_redirect=output_redirect, no_signature=True)

        if self.auto_clear:
            self.cache.pop(-1)

    def get_cache(self) -> list:
        cache = self.cache
        self.cache = list()
        return cache

    def debug(self, data) :
        self.print(data, output_location=self.OUT.DEBUG)
    
    def info(self, data):
        self.print(data, output_location=self.OUT.INFO)
    
    def warn(self, data):
        self.print(data, output_location=self.OUT.WARN)
    
    def error(self, data):
        self.print(data, output_location=self.OUT.ERROR)



    ######################################
    # PRINT FORMATTING
    ######################################

    @staticmethod
    def new_format_fulfillments(fulfillments, taken_courses) -> dict:

        formatted_fulfillments = Output.format_fulfillments(fulfillments, taken_courses)

        organized_fulfillments = Dict_Array(list_type='list')

        for fulfillment in formatted_fulfillments:
            tag = fulfillment.get('name', '').split('-')[0]
            
            if 'core' in tag.casefold():
                fulfillment.update({'display_style':'condensed'})
            else:
                fulfillment.update({'display_style':'separate'})

            organized_fulfillments.add(tag, fulfillment)

        return organized_fulfillments.dictionary
    
    @staticmethod
    def format_fulfillment_groups(groups) -> list:
        new_groups = []
        for group in groups:
            new_group = dict()
            new_group.update({'name': group.name, 'minimum_requirements': group.minimum_requirements, 'requirements':[e.name for e in group.requirements]})
            new_groups.append(new_group)
        return new_groups

    @staticmethod
    def format_fulfillments(fulfillments, taken_courses) -> list:
        
        formatted_fulfillments = list()

        # if fulfillment failed, typically due to no degree specified
        if isinstance(fulfillments, str):
            formatted = dict()
            formatted.update({'name':'No Degree Selected'})
            formatted.update({'content':False})
            formatted_fulfillments.append(formatted)
            return formatted_fulfillments
        
        for fulfillment in fulfillments.values():
            formatted = dict()
            formatted.update({'name':fulfillment.requirement.name})
            formatted.update({'content':True})
            formatted.update({'replacement':fulfillment.requirement.replacement})
            formatted.update({'position':fulfillment.requirement.importance})
            formatted.update({'required_count':fulfillment.get_required_count()})
            formatted.update({'actual_count':fulfillment.get_actual_count()})
            formatted.update({'required_credits':fulfillment.requirement.credits_required})
            formatted.update({'total_credits':fulfillment.get_total_credits()})
            formatted.update({'specifications':fulfillment.requirement.specifications})
            formatted.update({'original_specifications':fulfillment.requirement.original_formatted_specifications})
            fulfillment_set = [str(course) for course in fulfillment.fulfillment_set]
            fulfillment_set.sort()
            formatted.update({'fulfillment_set':fulfillment_set})
            wildcard_resolutions = fulfillment.requirement.wildcard_resolutions(taken_courses, True, True)
            wildcard_resolutions.convert_list_type('list')
            for key, _ in wildcard_resolutions.dictionary.items():
                wildcard_resolutions.insert(key, key, 0)
            formatted.update({'wildcard_resolutions':wildcard_resolutions.dictionary})

            formatted_fulfillments.append(formatted)

            formatted_fulfillments = sorted(formatted_fulfillments, key=lambda x:-x['position'])
        
        return formatted_fulfillments


    @staticmethod
    def format_fulfillments_dict(fulfillments, taken_courses) -> dict:
        
        formatted_fulfillments = dict()

        # if fulfillment failed, typically due to no degree specified
        if isinstance(fulfillments, str):
            return {'No Degree Selected':'No Degree Selected'}
        
        for fulfillment in fulfillments.values():
            formatted = dict()
            formatted.update({'name':fulfillment.requirement.display_name})
            formatted.update({'content':True})
            formatted.update({'replacement':fulfillment.requirement.replacement})
            formatted.update({'position':fulfillment.requirement.importance})
            formatted.update({'required_count':fulfillment.get_required_count()})
            formatted.update({'actual_count':fulfillment.get_actual_count()})
            formatted.update({'specifications':fulfillment.requirement.specifications})
            formatted.update({'original_specifications':fulfillment.requirement.original_formatted_specifications})
            fulfillment_set = [str(course) for course in fulfillment.fulfillment_set]
            fulfillment_set.sort()
            formatted.update({'fulfillment_set':fulfillment_set})

            wildcard_resolutions = fulfillment.requirement.wildcard_resolutions(taken_courses, True, True)
            wildcard_resolutions.convert_list_type('list')
            if fulfillment.requirement.track_resolutions is not None:
                wildcard_resolutions.extend(fulfillment.requirement.track_resolutions)
            for key, _ in wildcard_resolutions.dictionary.items():
                wildcard_resolutions.insert(key, key, 0)

            wildcard_resolutions_formatted = []
            for orig, alternatives in wildcard_resolutions.dictionary.items():
                wildcard_resolutions_formatted.append({'original': orig, 'alternatives': alternatives})

            formatted.update({'wildcard_resolutions':wildcard_resolutions_formatted})
            

            formatted_fulfillments.update({formatted['name'].casefold():formatted})
        
        return formatted_fulfillments
    

    @staticmethod
    def print_fulfillment(all_fulfillment:dict, as_dict=False) -> str:
        '''
        Print fulfillment dictionary in a neat string format
        '''
        if isinstance(all_fulfillment, str):
            if as_dict:
                return {'error':all_fulfillment}
            return all_fulfillment

        printout = ''
        if as_dict:
            printout = dict()
        fulfillments = list(all_fulfillment.values())
        fulfillments.sort()
        for status in fulfillments:
            if as_dict:
                printout.update({status.requirement.name:[str(e) for e in status.fulfillment_set]})
            else:
                printout += (f"  Template '{status.requirement.name}':" + \
                    f"\n    replacement: {status.requirement.replacement}, importance: {status.requirement.importance}" + \
                    f"\n    required count: {status.get_required_count()}" + \
                    f"\n    actual count: {status.get_actual_count()}" + \
                    f"\n    specifications: {status.requirement.specifications}" + \
                    f"\n    original specifications: {status.requirement.original_specifications}\n")
                simplified_fulfillment_set = set()
                for course in status.fulfillment_set:
                    simplified_fulfillment_set.add(course.name)
                printout += f"    fulfillment set: {simplified_fulfillment_set}\n"
        return printout
    
    @staticmethod
    def format_recommendations(recommendation:dict) -> list:

        formatted_recommendations = list()

        # if recommendation failed, typically due to no degree specified
        if isinstance(recommendation, str):
            return []

        requirements = list(recommendation.keys())
        requirements.sort()
        requirements.reverse()
        for requirement in requirements:
            for generated_requirement, fulfillment_courses in recommendation.get(requirement):
                formatted = dict()
                formatted.update({'name':generated_requirement.name})
                formatted.update({'specifications':generated_requirement.specifications})
                formatted.update({'courses_fulfilled':generated_requirement.elements_fulfilled})
                fulfillment_set = [str(e) for e in fulfillment_courses]
                del fulfillment_set[4:]
                if not requirement.hide_recommendations:
                    formatted.update({'fulfillment_set':fulfillment_set})
                else:
                    formatted.update({'fulfillment_set':[]})
                formatted_recommendations.append(formatted)

        return formatted_recommendations


    @staticmethod
    def print_recommendation(recommendation:dict, as_dict=False) -> str:
        '''
        Print recommendation dictionary in a neat string format
        '''
        if isinstance(recommendation, str):
            if as_dict:
                return {'error':recommendation}
            return recommendation
        
        if as_dict:
            printout = dict()
            requirements = list(recommendation.keys())
            requirements.sort()
            requirements.reverse()
            for requirement in requirements:
                for generated_requirement, fulfillment_courses in recommendation.get(requirement).items():
                    printout.update({f"{generated_requirement.name}":f"specifications: {generated_requirement.specifications}" + \
                    f"fulfillment courses: {[str(e) for e in fulfillment_courses]}"})
            return printout

        printout = ''
        requirements = list(recommendation.keys())
        requirements.sort()
        requirements.reverse()
        for requirement in requirements:
            printout += (f"\n  Original Template '{requirement.name}':" + \
                f"\n    replacement: {requirement.replacement}, importance: {requirement.importance}" + \
                f"\n    required count: {requirement.elements_required}" + \
                f"\n    specifications: {requirement.specifications}\n")
            
            for generated_requirement, fulfillment_courses in recommendation.get(requirement).items():
                printout += (f"\n    Generated Template '{generated_requirement.name}':" + \
                f"\n      specifications: {generated_requirement.specifications}" + \
                f"\n      fulfillment courses: {[str(e) for e in fulfillment_courses]}\n")

        return printout
