import os, sys
import xml.etree.cElementTree as ET
from xml.dom import minidom
from math import sqrt, fabs
'''
This script will FIX DIME LHE output to correct one
'''

class Particle:
  def __init__(self, line):
  
    info = line.split()
    self.isvalid = (len(info)==16) or (len(info)==14)
    if not self.isvalid: info=('0 '*14).split()
    self.idx = int(info[0])
    self.pid = int(info[2])
    self.status = int(info[1])
    self.mother1 = int(info[3])
    self.mother2 = int(info[4])
    self.color1 = int(info[5])
    self.color2 = int(info[6])
    self.px = float(info[7])
    self.py = float(info[8])
    self.pz = float(info[9])
    self.e = float(info[10])
    self.mass = float(info[11])
    self.lifetime = float(info[12])
    self.spin = float(info[13])
  
    # LHE file floating point precision
    nGidits1=len(info[10])-6  
    self.var_str = '%0.'+str(nGidits1)+'e'
	
  def isValid(self):
    return self.isvalid

  def getFloatWithSign(self, x):
    return self.var_str%(x+0.) if x<0 else '+'+self.var_str%fabs(x)

  def getLine(self):
    outline='     %4d %2d'%(self.pid,self.status)
    outline+=4*'    %d'%(self.mother1,self.mother2,self.color1,self.color2)
    outline+=3*' %s'%(self.getFloatWithSign(self.px),self.getFloatWithSign(self.py),self.getFloatWithSign(self.pz))
    outline+=2*(' '+self.var_str)%(self.e,self.mass)
    outline+=2*(' %1.1f')%(self.lifetime,self.spin)
    return outline

  def isPhoton(self):
    return self.pid==22
	
  def fromBeam(self):
    return self.status==-1

def GetInfoFromDIMEstdout(inputfile):
  print('Reads '+inputfile)
  with open(inputfile) as file:
    lines=file.readlines()
    energy_str=[x for x in lines if 'collider energy' in x][0]
    idx=lines.index(energy_str)
  if len([x for x in lines if 'meson mass (GeV)' in x]): idxPlus=4
  else: idxPlus=3
  for i in range(idxPlus): print('\t'+lines[idx+i].strip())
  energy = float(lines[idx].strip().split(':')[1])
  xsec = float(lines[idx+idxPlus-1].strip().split(':')[1].split()[0])
  xsecerr = float(lines[idx+idxPlus-1].strip().split(':')[1].split()[2])
  return energy, xsec*1e3, xsecerr*1e3

if (len(sys.argv) < 3):
    print('ERROR: missing input file\nUsage: python '+sys.argv[0]+' LHEFILE DIMESTDOUT')
    sys.exit(0)

inFile=sys.argv[1]
dime_stdout=sys.argv[2]
outfile='output.lhe'
Aew=7.29929980e-03
As=9.69544740e-02

if not os.path.exists(inFile):
  print('Cannot find'+inFile)
  sys.exit(0)

if not os.path.exists(dime_stdout):
  print('Cannot find'+dime_stdout)
  sys.exit(0)

energy, Xsec, XsecErr = GetInfoFromDIMEstdout(dime_stdout) 
ebeam = 0.5*energy
print(energy, Xsec, XsecErr)

# prepare lhe output file
root = ET.Element("LesHouchesEvents")
root.text = "\n"
root.tail = "\n"
header = ET.SubElement(root, "header")
header.text = "\n"
header.tail = "\n"
init = ET.SubElement(root, "init")
init.text = "\n"
init.text += "2212\t2212\t%0.8e\t%0.8e   -1   -1   -1   -1   4   1\n"%(ebeam,ebeam)
init.text += " %0.8e %0.8e %0.8e -1\n"%(Xsec,XsecErr,Xsec*1e-3)
init.tail = "\n"


# read lhe file
print('Reads '+inFile)
f = open(inFile) # Open file on read mode
lines = f.read().splitlines() # List with stripped line-breaks
f.close() # Close file

#write event content
idx=0; events=[]
while idx<len(lines):
  nup=int(lines[idx].split()[1])
  events.append(ET.SubElement(root, "event"))
  events[-1].text = "\n %d\t1 %0.8g %0.8g %0.8g %0.8g\n"%(nup,Xsec*1e-3,1.0,Aew,As)
  for i in range(nup):
    p = Particle(lines[idx+i+1])
    events[-1].text += p.getLine()+"\n"
  events[-1].tail = "\n"
  idx+=nup+1


#save new LHE file
tree = ET.ElementTree(root)
tree.write(outfile)
print('Writes '+outfile)
