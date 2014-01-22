import pylab as pl
import json
import filepaths as fp
import pprint
import scipy.stats as ss
import itertools
import likelihood_ratio as lr
import os.path as op


def drawDiscrepancy(filename,filenames):
  x_max = 0.0
  y_max = 0.0
  for fn in itertools.chain(*filenames):
    try:
      f = open(fn,'r')
    except IOError:
      continue
    #except TypeError:
    #  #print "squick", fn,"\n\n"
    #  continue
    data = json.load(f)
    x_max = max(x_max,data["original_width"]["IQ"])
    y_max = max(y_max,data["mean"]["IQ"]+data["standard_deviation"]["IQ"])
  for plot_filename,fns in zip(filename,filenames):
    plot_points = []
    for fn in fns:
      try:
        f = open(fn,'r')
      except IOError:
        continue
      data = json.load(f)
      plot_points.append({"original":data["original_width"]["IQ"],
                         "mean":data["mean"]["IQ"],
                         "sd":data["standard_deviation"]["IQ"] })
    xs = [x["original"] for x in plot_points]
    ys = [x["mean"] for x in plot_points]
    es = [x["sd"] for x in plot_points]
    y_plus_es = [y+e for (y,e) in zip(ys,es)]
    y_minus_es = [y-e for (y,e) in zip(ys,es)]
    yslope, yintercept,ycorrelationR,yprobability,ysterrest = ss.linregress(xs,ys)
    ypslope, ypintercept,ypcorrelationR,ypprobability,ypsterrest = ss.linregress(xs,y_plus_es)
    ymslope, ymintercept,ymcorrelationR,ymprobability,ymsterrest = ss.linregress(xs,y_minus_es)
    fs = [x*yslope+yintercept for x in xs]
    my_lr = lr.likelihood_ratio(xs,fs,ys,es)

    pl.clf()

    pl.errorbar(xs,ys,yerr=es,fmt='o')
    max_coord = max(y_max,x_max)*1.05
    pl.plot([0,max_coord],[0,max_coord],'g')
    pl.plot([0,max_coord],[yintercept,yintercept+max_coord*yslope],'m')
    pl.xlim(0,x_max*1.05)
    pl.ylim(0,y_max*1.05)
    pl.axes().set_aspect('1','box')
    pl.xlabel("Original width")
    pl.ylabel("Predicted width +- std")
    title = op.split(plot_filename)[1]
    pl.title(title+" slope = {0:.3}, r = {1:.3}, lambda = {2:.3}".format(yslope,ycorrelationR,my_lr))
    new_filename = fp.down_folder(plot_filename,"plots")
    new_filename = fp.down_folder(new_filename,"discrepancies")
    new_filename = fp.replace_extension(new_filename,".png")
    fp.create_folder_if_nonexistent(new_filename)
    pl.savefig(new_filename)
    print new_filename

def drawGroupAngleDistribution(filename,filenames, control=False):
  data = []
  for fn in filenames:
    try:
      f = open(fn,'r')
    except IOError:
      return
    data.append(json.load(f))
    data[-1]["filename"] = fn
  if control:
    cbins,cweights = angleDistributionControlData(180)
    data.append({"bin_boundaries":cbins,"bin_weights":cweights,"filename":"Control"})
  default_bounds = data[0]["bin_boundaries"]
  for d in data:
    bounds = d["bin_boundaries"]
    if bounds != default_bounds:
      print "Warning: bin boundaries differ in group angle distribution",data[0]["filename"],d["filename"]
    d["bounds90"] = [b-90 for b in bounds]
    bin_widths = [y-x for x,y in zip(bounds[:-1],bounds[1:])]
    weights = d["bin_weights"]
    total = sum(weights)
    d["fractions"] = [weight/(width*total) for weight,width in zip(weights,bin_widths)]
    d["cumul_fractions"] = pl.cumsum(d["fractions"])
  new_filename = fp.replace_extension(fp.down_folder(filename,"plots"),".png")
  new_filename = fp.down_folder(new_filename,"angles")
  drawGroupAngleDistributionSub(fp.append_to_filename(new_filename,"_f"),
                                [d["bounds90"] for d in data],
                                [d["fractions"] for d in data],
                                "Angle distributions",
                                "Angle to plane", "Probability density")
  drawGroupAngleDistributionSub(fp.append_to_filename(new_filename,"_cf"),
                                [d["bounds90"] for d in data],
                                [d["cumul_fractions"] for d in data],
                                "Cumulative angle distributions",
                                "Angle to plane", "Cumulative probability density")
    
def drawGroupAngleDistributionSub(filename,boundses,valueses,title,xlabel,ylabel):
  fp.create_folder_if_nonexistent(filename)
  pl.clf()
  for bounds, values in zip(boundses, valueses):
    xs,ys = generateAngleCurve(values,bounds)
    pl.plot(xs,ys)
  pl.xlim(min([min(bounds) for bounds in boundses]),max([max(bounds) for bounds in boundses]))
  pl.ylim(0,max([max(values) for values in valueses]))
  genericDrawTasks(title,xlabel,ylabel)
  pl.savefig(filename)
  print filename, "saved"

def angleDistributionControlData(bin_count):
  bins = [180.0*x/float(bin_count) for x in range(bin_count + 1)]
  bin_pairs = zip(bins[:-1],bins[1:])
  weights = [pl.cos(b*pl.pi/180) - pl.cos(a*pl.pi/180) for (a,b) in bin_pairs]
  total = sum(weights)
  weights = [w/total for w in weights]
  return bins,weights
  

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
  
def drawAngleDistributionSub(filename,bounds,values,title,xlabel,ylabel,clear=True):
  fp.create_folder_if_nonexistent(filename)
  xs,ys = generateAngleCurve(values,bounds)
  if clear:
    pl.clf()
  pl.plot(xs,ys)
  pl.xlim(min(bounds),max(bounds))
  pl.ylim(0,max(values))
  genericDrawTasks(title,xlabel,ylabel)
  pl.savefig(filename)
  print filename, "saved"

def generateAngleCurve(values, bounds):
  xs = []
  ys = []
  for v in values:
    ys.append(v)
    ys.append(v)
  for a,b in zip(bounds[:-1],bounds[1:]):
    xs.append(a)
    xs.append(b)
  return xs,ys

def genericDrawTasks(title,xlabel,ylabel):
  pl.xlabel(xlabel)
  pl.ylabel(ylabel)
  pl.title(title)

def drawJointDistributionSub(filename,H,xedges,yedges,cond=""):
  if cond == "":
    HH = H
  elif cond == "x":
    HH = H/H.max(axis=1)[:,pl.newaxis]
  elif cond == "y":
    HH = H/H.max(axis=0)
  pl.clf()
  pl.pcolormesh(pl.array(xedges),pl.array(yedges), HH,cmap='Greys')
  #todo generate xlabel, ylabel in C++
  if cond == "": title_append = ""
  else: title_append = ", normalised along " + cond + " axis"
  pl.title(filename+title_append)
  new_filename = fp.replace_extension(fp.down_folder(filename,"plots"),".png")
  new_filename = fp.down_folder(new_filename,"joint_dist")
  if cond != "":
    new_filename = fp.append_to_filename(new_filename,"_"+cond)
  fp.create_folder_if_nonexistent(new_filename)
  pl.colorbar()
  pl.savefig(new_filename,bbox_inches=0)
  print new_filename, "saved"

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
  bin_boundaries_x = [min_x + (max_x-min_x)*x/bin_count_x for x in range(bin_count_x+1)]
  bin_boundaries_y = [min_y + (max_y-min_y)*y/bin_count_y for y in range(bin_count_y+1)]
  drawJointDistributionSub(filename,weights,bin_boundaries_x,bin_boundaries_y)
  drawJointDistributionSub(filename,weights,bin_boundaries_x,bin_boundaries_y,"x")
  drawJointDistributionSub(filename,weights,bin_boundaries_x,bin_boundaries_y,"y")

def drawProjection(filename,ranges):
  try:
    f = open(filename,'r')
  except IOError:
    return
  data = json.load(f)
  points, lines = interpretProjection(data["points"], data["lines"])
  x_centre = (min(points["x"]+lines["x"]) + max(points["x"]+lines["x"]))/2.0
  y_centre = (min(points["y"]+lines["y"]) + max(points["y"]+lines["y"]))/2.0
  params = {
            "xmin":x_centre - (ranges[data["x_label"]]/2.0),
            "xmax":x_centre + (ranges[data["x_label"]]/2.0),
            "ymin":y_centre - (ranges[data["y_label"]]/2.0),
            "ymax":y_centre + (ranges[data["y_label"]]/2.0),
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

def interpretProjection(datapoints, datalines):
  points = {"x":[],"y":[]}
  for x,y in [(d["x"],d["y"]) for d in datapoints]:
      points["x"].append(x)
      points["x"].append(pl.NaN)
      points["y"].append(y)
      points["y"].append(pl.NaN)
  lines = {"x":[],"y":[]}
  for (sx,sy),(ex,ey) in [((d["start"]["x"],d["start"]["y"]),(d["end"]["x"],d["end"]["y"])) for d in datalines]:
      lines["x"].append(sx)
      lines["x"].append(ex)
      lines["x"].append(pl.NaN)
      lines["y"].append(sy)
      lines["y"].append(ey)
      lines["y"].append(pl.NaN)
  return points, lines

def drawProjectionSub(points,lines,params):
  pl.clf()
  pl.plot(lines["x"],lines["y"],marker=' ')
  pl.plot(points["x"],points["y"],marker='x',color='r')
  genericDrawTasks(params["title"],params["x_label"],params["y_label"])
  pl.axes().set_aspect(1.0,adjustable='box')
  pl.xlim(params["xmin"],params["xmax"])
  pl.ylim(params["ymin"],params["ymax"])


def drawGroupProjection(filenames):
  data = []
  for fn in filenames:
    try:
      f = open(fn,'r')
    except IOError:
      continue
    data.append(json.load(f))
    data[-1]["points"],data[-1]["lines"] = interpretProjection(data[-1]["points"],data[-1]["lines"])
  max_ranges = {}
  for d in data:
    range_x = max(d["lines"]["x"]) - min(d["lines"]["x"])
    range_y = max(d["lines"]["y"]) - min(d["lines"]["y"])
    xl = d["x_label"]
    yl = d["y_label"]
    max_ranges[xl] = max(range_x,max_ranges.setdefault(xl,range_x))
    max_ranges[yl] = max(range_y,max_ranges.setdefault(yl,range_y))
  for fn in filenames: #inefficient but that doesn't matter much here
    drawProjection(fn,max_ranges)
  
  
  
  