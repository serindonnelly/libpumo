import os.path as op
import os

def up_folder(path):
  fol,file=op.split(path)
  new_fol = op.split(fol)
  new_path = op.join(new_fol,file)
  return new_path

def down_folder(path,subfolder):
  fol,file = op.split(path)
  new_fol = op.join(fol,subfolder)
  new_path = op.join(new_fol,file)
  return new_path

def append_to_filename(path,suffix):
  file,ext = op.splitext(path)
  new_file = file+suffix
  new_path = new_file+ext
  return new_path

#seriously?
#def add_extension(path,ext):
#  return path+ext

def replace_extension(path,ext):
  file,old_ext = op.splitext(path)
  new_path = file + ext
  return new_path

def create_folder_if_nonexistent(path):
  fol,file = op.split(path)
  if not op.exists(fol):
    os.makedirs(fol)

def totally_normalise(path):
  new_path = path.replace('\\', '/')
  new_path = op.normpath(new_path)
  return new_path
