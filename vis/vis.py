import pylab
import json
import sys
import itertools
import pprint
import drawing

class AnalysisStack:
  def __init__(self, filename):
    stackFile = open(filename,'r')
    stackJSON = json.load(stackFile)
    self.readDirectory = stackJSON["read_directory"]
    self.writeDirectory = stackJSON["write_directory"]
    analysisStack = stackJSON["analysis_stack"]
    self.expandedStack = {}
    self.expandAnalysisStack(analysisStack)

  def drawAll(self):
    for idx,analysis in self.expandedStack.items():
      #pprint.pprint(analysis)
      if "routine" not in analysis:
        continue
      elif analysis["routine"] == u"angledistribution":
        drawing.drawAngleDistribution(analysis["filename"])
      #elif analysis["routine"] in [u"lpproject",u"lsproject",u"spproject"]:
      #  drawing.drawProjection(analysis["filename"])
      elif analysis["routine"] == u"heightdistribution":
        drawing.drawJointDistribution(analysis["filename"])
      elif analysis["routine"] == u"parentdistribution":
        drawing.drawJointDistribution(analysis["filename"])
      elif analysis["routine"] == u"distancedistribution":
        drawing.drawJointDistribution(analysis["filename"])
      elif u"group" in analysis["routine"]:
        self.drawGroup(analysis)
  
  def drawGroup(self,analysis):
    filenames = []
    for input in analysis["list"]:
      filenames.append(self.expandedStack[input]["filename"])
    if analysis["routine"] == u"groupangledistribution":
      drawing.drawGroupAngleDistribution(analysis["filename"],filenames,True)
    elif analysis["routine"] == u"groupprojection":
      drawing.drawGroupProjection(filenames)

  def expandAnalysisStack(self,stack):
    for analysis in stack:
      if analysis["routine"] == "makelist":
        self.addList(analysis["routine"],analysis["to"],analysis["from"],analysis["ids"])
      elif u"group" in analysis["routine"]: #make no analyses with "group" in their routine unless they should do this
        self.addList(analysis["to"],analysis["from"],analysis["ids"])
      elif analysis["routine"] == "specifyfile":
        self.addFile(analysis["to"],analysis["from"],analysis["ids"])
      else:
        self.addProcessings(analysis["routine"],analysis["to"],analysis["from"],analysis["ids"])

  def addProcessings(self,routine,toVal,fromVal,idsVal):
    for i in self.expandList(idsVal):
      a = {"routine":routine,"inputs":[],"filename":self.writeDirectory+toVal+i+".json"}
      for f in self.expandList(fromVal):
        a["inputs"].append(f+i)
      self.expandedStack[toVal+i] = a

  def addFile(self,toVal,fromVal,idsVal):
    for f,i in zip(self.expandList(fromVal),self.expandList(idsVal)):
      self.expandedStack[toVal+i] = {"swc_filename":self.readDirectory+f}

  def addList(self,routineVal,toVal,fromVal,idsVal):
    self.expandedStack[toVal] = {"routine":routineVal,"list":self.makeList(fromVal,idsVal)}

  def makeList(self,fromVal,idsVal):
    fromFull = self.expandList(fromVal)
    idsFull = self.expandList(idsVal)
    return [x+y for x in fromFull for y in idsFull]
    
  def expandList(self,stringOrList):
    if isinstance(stringOrList, basestring):
      if len(stringOrList) == 0:
        return [stringOrList]
      if stringOrList[0] != '@':
        return [stringOrList]
      else:
        return self.expandedStack[stringOrList[1:]]["list"]
    else:
      return reduce(lambda L,x: L+x, map(self.expandList,stringOrList),[])
                

def main():
    #filename = sys.argv[1]
    filename = "../analysis_stack_all.json"
    stack = AnalysisStack(filename)
    #pprint.pprint(stack.expandedStack)
    #getch()
    stack.drawAll()


if __name__ == "__main__":
    main()