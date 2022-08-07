import difflib
import re

def parseRed(file):
    f = open(file, 'rb')
    red = f.read()
    red = red.decode('utf-8')
    f.close()
    lines = red.split('\n')
    inVariable = False
    variables = {}
    for line in lines:
        if line == "":
            continue
        if not inVariable:
            varNameRE = re.match('^.*::$', line)
            if varNameRE != None:
                varName = varNameRE.string
                variables[varName] = []
                inVariable = True
                continue
        else:
            lineRE = re.match("^[\t\s]*(text |line |cont |para )\"(.*)\"$",line)
            if lineRE != None:
                func = lineRE.groups()[0].strip()
                text = lineRE.groups()[1]
                variables[varName].append([func,text])
                continue
            lineRE = re.match("^[\t\s]*(done|prompt)$",line)
            if lineRE != None:
                endfunc = lineRE.groups()[0].strip()
                variables[varName].append([endfunc,""])
                inVariable = False
                continue
            else:
                print(f"Error parsing line: {line}")
    return variables

def parseFireRed(file):
    f = open(file, 'r')
    firered = f.read()
    f.close()
    lines = firered.split('\n')
    inVariable = False
    variables = {}
    for line in lines:
        if line == "":
            continue
        if not inVariable:
            varNameRE = re.match('^.*::$', line)
            if varNameRE != None:
                varName = varNameRE.string
                variables[varName] = []
                inVariable = True
                continue
        lineRE = re.match("^[\t\s]*\.string \"(.*)(\\\\n|\\\\l|\\\\p|\\$)+\"$",line)
        if lineRE != None:
            text = lineRE.groups()[0]
            func = lineRE.groups()[1]
            variables[varName].append([func,text])
            if func == '$':
                inVariable = False
            continue
        else:
            print(f"Error parsing line: {line}")
    return variables

def getRefrences(variables):
    refrences = {}
    for variable in variables.keys():
        fulltext = ""
        for line in variables[variable]:
            fulltext = fulltext  + " " + line[1]
        refrences[fulltext] = variable
    return refrences

def findMapping(fireRedRefs, redRefs):
    mappings = {}
    for fireRedText in fireRedRefs.keys():
        fireRedVariable = fireRedRefs[fireRedText]
        for redText in redRefs.keys():
            redVariable = redRefs[redText]
            score = difflib.SequenceMatcher(None, fireRedText, redText).ratio()
            print(f"S {fireRedVariable} against {redVariable} {score}")
            if score > 0.8:
                mappings[fireRedVariable] = redVariable
                break
    return mappings

def convertRedToFireRed(variableName, redList):
    out = variableName + "\n"
    for line in redList:
        if line[0] == 'text' or line[0] == 'line' or line[0] == 'done':
            out += "\t.string \"" + line[1] + "\""
        if line[0] == 'para':
            out += "\t.string \"" + line[1] + '\\p\"'
        if line[0] == 'cont':
            out += "\t.string \"" + line[1] + "\\n\""
        out += "\n"
    return out


redVariables = parseRed('RedsHouse1F.asm.txt')
fireRedVariables = parseFireRed('text.inc.txt')
redHebrewVariables = parseRed('RedsHouse1F_Hebrew.asm.txt')

redRefrences = getRefrences(redVariables)
fireRedReferences = getRefrences(fireRedVariables)

mappings = findMapping(fireRedReferences, redRefrences)

all = ""
for m in mappings:
    all += convertRedToFireRed(m, redHebrewVariables[mappings[m]])

f = open('translated.txt','wb')
f.write(all.encode('utf-8'))
f.close()


