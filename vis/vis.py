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

  def draw(self,idx):
    analysis = self.expandedStack[idx]
    #pprint.pprint(analysis)
    if "routine" not in analysis:
      return
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
    elif analysis["routine"] == u"compareangledistribution":
      drawing.drawAngleComparison(analysis["filename"],
                                  self.expandedStack[analysis["inputs"][0]]["filename"],
                                  self.expandedStack[analysis["inputs"][1]]["filename"])
    elif analysis["routine"] == u"compareparentdistribution":
      drawing.drawParentComparison(analysis["filename"],
                                  self.expandedStack[analysis["inputs"][0]]["filename"],
                                  self.expandedStack[analysis["inputs"][1]]["filename"])
    elif analysis["routine"] == u"comparedistancedistribution":
      drawing.drawDistanceComparison(analysis["filename"],
                                  self.expandedStack[analysis["inputs"][0]]["filename"],
                                  self.expandedStack[analysis["inputs"][1]]["filename"])
    elif analysis["routine"] == u"#discrepancy":
      drawing.drawDiscrepancy([self.expandedStack[idx]["filename"] for idx in analysis["inputs"]],
                              [[self.expandedStack[i]["filename"] for i in self.expandedStack[idx]["inputs"]]
                                for idx in analysis["inputs"]])

  def drawAll(self,first=[],last=[]):
    ffirst = frozenset(first)
    llast = frozenset(last)
    for idx in ffirst:
      self.draw(idx)
    for idx in self.expandedStack.keys():
      if idx not in ffirst and idx not in llast: self.draw(idx)
    for idx in llast:
      self.draw(idx)
    

  
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
        self.addList(analysis["routine"],analysis["to"],analysis["from"],analysis["ids"])
      elif analysis["routine"] == "specifyfile":
        self.addFile(analysis["to"],analysis["from"],analysis["ids"])
      elif analysis["routine"] == "#discrepancy":
        self.addDiscrepancy(analysis["to"],analysis["from"],analysis["ids"])
      else:
        self.addProcessings(analysis["routine"],analysis["to"],analysis["from"],analysis["ids"])
        
  def addDiscrepancy(self,toVal,fromVal,idsVal):
    fromList = self.expandList(fromVal)
    for fromSingle in fromList:
      for name, lst in idsVal.items():
        self.expandedStack[toVal+fromSingle+name] = {"inputs":[fromSingle+idx for idx in self.expandList(lst)],
                                          "filename":self.writeDirectory+toVal+fromSingle+name+".json"
                                          }
    self.expandedStack[toVal] = {"routine":"#discrepancy",
                                 "inputs":[toVal+f+i for f in fromList for i in idsVal.keys()]
                                 }

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
    self.expandedStack[toVal] = {"routine":routineVal,
                                 "list":self.makeList(fromVal,idsVal),
                                 "filename":self.writeDirectory+toVal}

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
    stack.drawAll(first=["angleMC_verify_compare_unred_oko_27"],last=["#discrepancy_","all_projections"])


if __name__ == "__main__":
    main()