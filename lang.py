#!/usr/bin/python3
import argparse
import subprocess
from xml.dom import minidom
import sys
import os

current_dir = os.path.dirname(os.path.realpath(__file__))

# Works only on Fedora (tools are called lupdate-qt5 and lrelease-qt5)
lupdate = "lupdate-qt5"
lrelease = "lrelease-qt5"

def getText(element):
    if element.firstChild is None:
        return None
    text = element.firstChild
    if text.nodeType != text.TEXT_NODE:
        return None
    return text.wholeText

def copy(element, doc):
    element_copy = doc.createElement(element.tagName)
    for attribute in element.attributes.keys():
        element_copy.setAttribute(attribute, element.getAttribute(attribute))
    # Copy text
    text = getText(element)
    if text is None:
        return element_copy
    text_copy = doc.createTextNode(text)
    element_copy.appendChild(text_copy)
    return element_copy

def do_push_subproject(folder, file):
    # Files
    tmp_file = "%s/%s/translations/%s-tmp.ts" % (current_dir, folder, file)
    final_file = "%s/%s/translations/%s.ts" % (current_dir, folder, file)
    working_dir = "%s/%s" % (current_dir, folder)
    
    # Execute lupdate
    subprocess.call([lupdate, working_dir, "-ts", tmp_file])
    
    # Open the translation file and do a fake translation
    doc = minidom.parse(tmp_file)
    ts = doc.documentElement
        
    final_doc = minidom.Document()
    imp =  minidom.getDOMImplementation('')
    dt = imp.createDocumentType('TS', '', '')
    final_doc.appendChild(dt)
    
    # Copy TS
    final_ts = copy(ts, doc)
    final_ts.setAttribute("language", "en")
    final_doc.appendChild(final_ts)
    
    # Copy contexts
    contexts = ts.getElementsByTagName("context")
    for context in contexts:
        final_context = copy(context, doc)
        final_ts.appendChild(final_context)
        
        # Copy context name
        names = context.getElementsByTagName("name")
        for name in names:
            final_context.appendChild(copy(name, doc))
        
        # Copy messages
        messages = context.getElementsByTagName("message")
        for message in messages:
            final_message = copy(message, doc)
            final_context.appendChild(final_message)
            
            # Copy location / source / extra comment
            locations = message.getElementsByTagName("location")
            for location in locations:
                final_message.appendChild(copy(location, doc))
            sources = message.getElementsByTagName("source")
            for source in sources:
                sourceText = getText(source)
                final_message.appendChild(copy(source, doc))
            extra_comments = message.getElementsByTagName("extracomment")
            for extra_comment in extra_comments:
                final_message.appendChild(copy(extra_comment, doc))
            
            translation = doc.createElement("translation")
            final_message.appendChild(translation)
            if message.hasAttribute("numerus"):
                # We need to add <numerusform>text</numerusform> twice
                for i in range(0, 2):
                    numerus_form = doc.createElement("numerusform")
                    text_copy = doc.createTextNode(sourceText)
                    numerus_form.appendChild(text_copy)
                    translation.appendChild(numerus_form)
            else:
                text_copy = doc.createTextNode(sourceText)
                translation.appendChild(text_copy)
    
    f = open(final_file, 'w')
    final_doc.writexml(f, encoding="utf-8")
    f.close()
    
    subprocess.call(["rm", tmp_file])

def do_push():
    do_push_subproject("src/accounts", "accounts")
    subprocess.call(["tx", "push", "-s"])

def do_pull_subproject(folder, file):
    working_dir = "%s/%s/translations" % (current_dir, folder)
    ts_file = "%s/%s/translations/%s.ts" % (current_dir, folder, file)
    qm_file = "%s/%s/translations/%s-engineering-english.qm" % (current_dir, folder, file)
    
    subprocess.call(["rm *.ts"], cwd=working_dir, shell=True)
    subprocess.call(["%s -idbased *.ts" % lrelease], cwd=working_dir, shell=True)
    subprocess.call(["rm *.ts"], cwd=working_dir, shell=True)
    subprocess.call([lupdate, working_dir, "-ts", ts_file])
    subprocess.call(["%s -idbased %s -qm %s" % (lrelease, ts_file, qm_file)], cwd=working_dir, shell=True)
    subprocess.call(["rm *.ts"], cwd=working_dir, shell=True)

def do_pull(perc):
    subprocess.call(["tx", "pull", "-a", "--minimum-perc=%s" % perc])
    do_pull_subproject("src/accounts", "accounts")
    

parser = argparse.ArgumentParser(description='Friends translation management script')
parser.add_argument('--push', action='store_true', help='Push source translation file')
parser.add_argument('--pull', action='store_true', help='Pull translation file')
parser.add_argument('percentage', metavar='percentage', type=int, nargs='?',
                    help='Percentage of translation required')

args = parser.parse_args()
push = args.push
pull = args.pull
percentage = args.percentage

if push and pull or (not push and not pull):
    parser.print_help()
    sys.exit(1)

if percentage is not None and not pull:
    parser.print_help()
    sys.exit(1)
   
if percentage is not None:
    if percentage < 0 or percentage > 100:
        parser.print_help()
        sys.exit(1)
else:
    percentage = 100
if push:
    do_push()
    sys.exit(0)

if pull:
    do_pull(percentage)
    sys.exit(0)