import pylab as pl
import json
import filepaths as fp

def drawAngleDistribution(filename):
  try:
    f = open(filename,'r')
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
                           "Angle to plane","Cumulative probability density")
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_cd"),
                           bounds90,cumul_densities,
                           "Cumulative angle histogram",
                           "Angle to plane","Cumulative length per degree")
  drawAngleDistributionSub(fp.append_to_filename(new_filename,"_f"),
                           bounds90,fractions,
                           "Angle distribution",
                           "Angle to plane","Probability density")
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
  genericDrawTasks(title,xlabel,ylabel)
  pl.savefig(filename)
  print filename, "saved"

def genericDrawTasks(title,xlabel,ylabel):
  pl.xlabel(xlabel)
  pl.ylabel(ylabel)
  pl.title(title)

def drawJointDistribution(filename):
  try:
    f = open(filename,'r')
  except IOError:
    return
  data = json.load(f)
  bin_count_x = data["bin_count_x"]
  bin_count_y = data["bin_count_y"]
  max_x = data["max_x"]
  max_y = data["max_y"]
  min_x = data["min_x"]
  min_y = data["min_y"]
  weights_flat = data["weights"]
  weights = pl.zeros((bin_count_y,bin_count_x))
  count = 0
  for x in range(bin_count_x):
    for y in range(bin_count_y):
      weights[y][x] = weights_flat[count]
      count += 1
  pl.clf()
  bin_boundaries_x = [min_x + (max_x-min_x)*x/bin_count_x for x in range(bin_count_x+1)]
  bin_boundaries_y = [min_y + (max_y-min_y)*y/bin_count_y for y in range(bin_count_y+1)]
  pl.pcolormesh(pl.array(bin_boundaries_x),pl.array(bin_boundaries_y), weights,cmap='Greys')
  #todo generate xlabel, ylabel in C++
  pl.title(filename)
  new_filename = fp.replace_extension(fp.down_folder(filename,"plots"),".png")
  new_filename = fp.down_folder(new_filename,"joint_dist")
  fp.create_folder_if_nonexistent(new_filename)
  pl.colorbar()
  pl.savefig(new_filename,bbox_inches=0)
  print new_filename, "saved"
  
def drawProjections(filenames):
  pass

def drawProjection(filename):
  try:
    f = open(filename,'r')
  except IOError:
    return
  data = json.load(f)
  points = {"x":[],"y":[]}
  for x,y in [(d["x"],d["y"]) for d in data["points"]]:
      points["x"].append(x)
      points["x"].append(pl.NaN)
      points["y"].append(y)
      points["y"].append(pl.NaN)
  lines = {"x":[],"y":[]}
  for (sx,sy),(ex,ey) in [((d["start"]["x"],d["start"]["y"]),(d["end"]["x"],d["end"]["y"])) for d in data["lines"]]:
      lines["x"].append(sx)
      lines["x"].append(ex)
      lines["x"].append(pl.NaN)
      lines["y"].append(sy)
      lines["y"].append(ey)
      lines["y"].append(pl.NaN)
  params = {
            "xmin":min(lines["x"]),
            "xmax":max(lines["x"]),
            "ymin":min(lines["y"]),
            "ymax":max(lines["y"]),
            "x_label":data["x_label"],
            "y_label":data["y_label"],
            "title":"Projection of " + filename
            }
  drawProjectionSub(points, lines, params)
  new_filename = fp.replace_extension(fp.down_folder(filename,"plots"),".png")
  new_filename = fp.down_folder(new_filename,"projections")
  new_filename = fp.append_to_filename(new_filename,"_"+data["x_label"][0]+data["y_label"][0])
  fp.create_folder_if_nonexistent(new_filename)
  pl.savefig(new_filename,bbox_inches=0)
  print new_filename, "saved"



def drawProjectionSub(points,lines,params):
  pl.clf()
  pl.plot(lines["x"],lines["y"],marker=' ')
  pl.plot(points["x"],points["y"],marker='x',color='r')
  genericDrawTasks(params["title"],params["x_label"],params["y_label"])
  pl.axes().set_aspect('equal', 'datalim')
