#!/usr/bin/python
import sys
import os

try:
  import_dir = sys.argv[1]
except:
  print "Please specify the location to import from."
  sys.exit()

baseDir = os.path.dirname(sys.argv[0])
baseDir = os.path.abspath( baseDir )
baseDir = os.path.join( baseDir+os.sep+'..'+os.sep)
baseDir = os.path.abspath( baseDir )
os.system("cp "+os.path.join(baseDir, "midi.py") + " /tmp/")

def sort_string(a, b):
  if ( int(a[:-3]) < int(b[:-3]) ):
    return -1
  return 1

def addlevel(campaignfile, name, level, minbpm, maxbpm):
  campaignfile.write('\n')
  campaignfile.write(name+'\n')
  campaignfile.write(str(minbpm)+'\n')
  campaignfile.write(str(maxbpm)+'\n')
  files=os.listdir( os.path.join(import_dir, level) )
  files.sort(sort_string)
  if not(os.path.exists(os.path.join(baseDir, "rhythm", level))):
    os.mkdir(os.path.join(baseDir, "rhythm", level))
#  os.chdir(os.path.join(baseDir, "rhythm", level))
  os.chdir("/tmp")
  for file in files:
    base = file[:-3]
    campaignfile.write( 'rhythm/'+level+'/'+base+'.png\n' )
    #campaignfile.write( level+'/'+base+'.png\n' )
    ly = 'tmp-ly.ly'
    lybase = ly[:-3]
    os.system("cat " + baseDir + "/header.ly "
      + os.path.join(import_dir,level,file) + " > " + ly)
    os.system("convert-ly -e  " + ly)
#    os.system("lilypond -dbackend=svg " + ly)
#    os.system("~/Apps/Inkscape.app/Contents/Resources/bin/inkscape "
#      + "--export-png=" + base + ".png "
#      + "--export-background=00 --export-width=628 "
#      + "--export-area-drawing "
#      + lybase + ".svg")

#   don't use because qt shows it bigger.  :(
#    os.system("lilypond -dresolution=200 -dpreview " + ly)
    os.system("lilypond -dpreview " + ly)
    os.system(os.path.join(baseDir, "makeexample.py") + " "
      + lybase + ".midi > "
      + os.path.join(baseDir, "rhythm", level, base + ".exp"))
    os.system("cp "+lybase+".preview.png "
      + os.path.join(baseDir,"rhythm",level, base+'.png'))

pass_grade = 80
min_pass = 2
campaignfile = open( 'rhythms.txt', 'w')
campaignfile.write( str(pass_grade)+'\n' )
campaignfile.write( str(min_pass)+'\n' )
addlevel(campaignfile, "Novice", "quarter00", 90, 90)
addlevel(campaignfile, "Simple", "eighth01", 60, 120)
addlevel(campaignfile, "Simple", "sixteen01", 60, 90)
addlevel(campaignfile, "Easy", "sixteen02", 60, 90)
addlevel(campaignfile, "Easy", "rest01", 60, 90)
addlevel(campaignfile, "Easy", "long01", 60, 90)

addlevel(campaignfile, "Moderate", "triplet01", 60, 90)
addlevel(campaignfile, "Moderate", "triplet02", 60, 90)
addlevel(campaignfile, "Hard", "triplet03", 60, 90)
addlevel(campaignfile, "Hard", "rest02", 60, 90)
addlevel(campaignfile, "Challenging", "long02", 60, 90)
campaignfile.write('\n')
campaignfile.close()


