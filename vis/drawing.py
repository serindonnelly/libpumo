import pylab as pl
import json
import filepaths as fp

def drawAngleDistribution(filename):
  try:
    f = open(filename)
  except IOError:
    return
  data = json.load(f)
  bounds = data["bin_boundaries"]
  bounds90 = [b-90 for b in bounds]
  bin_widths = [y-x for x,y in zip(bounds[:-1],bounds[1:])]
  weights = data["bin_weights"]
  total = sum(weights)
  densities = [weight/width for weight,width in zip(weights,bin_widths)]
  fractions = [density/total for density in densities]
  cumul_densities = pl.cumsum(densities)
  cumul_fractions = pl.cumsum(fractions)
  new_filename = fp.replace_extension(fp.down_folder(filename,"plots"),".png")
  new_filename = fp.down_folder(new_filename,"angles")
                  
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_cf"),
                           bounds90,cumul_fractions,
                           "Cumulative angle distribution",
                           "Angle to plane","Cumulative probability")
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_cd"),
                           bounds90,cumul_densities,
                           "Cumulative angle histogram",
                           "Angle to plane","Cumulative density")
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_f"),
                           bounds90,fractions,
                           "Angle distribution",
                           "Angle to plane","Probability")
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_d"),
                           bounds90,densities,
                           "Angle histogram",
                           "Angle to plane","Length per degree")
  
def drawAngleDistributionSub(filename,bounds,values,title,xlabel,ylabel):
  fp.create_folder_if_nonexistent(filename)
  xs = []
  ys = []
  for v in values:
    ys.append(v)
    ys.append(v)
  for a,b in zip(bounds[:-1],bounds[1:]):
    xs.append(a)
    xs.append(b)
  pl.clf()
  pl.plot(xs,ys)
  pl.xlim(min(bounds),max(bounds))
  pl.ylim(0,max(values))
  genericDrawTasks(filename,title,xlabel,ylabel)
  pl.savefig(filename)
  print filename, "saved"

def genericDrawTasks(filename,title,xlabel,ylabel):
  pl.xlabel(xlabel)
  pl.ylabel(ylabel)
  pl.title(title)

def drawJointDistribution(filename):
  pass

def drawProjections(filenames):
  pass

def drawProjection(filename,params):
  pass
