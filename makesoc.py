
import os
import sys
import shutil
import urllib
import zipfile



out_path='./'
bin_file_name='rtthread.bin'
pack_path='./pack'
out_file='./rtthread_air105'

cwd_path = os.getcwd()
if __name__=='__main__':
    
    print(cwd_path)
    if os.path.exists(out_file+'.soc'):
        os.remove(out_file+'.soc')

    if not os.path.exists(pack_path+'/bootloader.bin'):
        urllib.urlretrieve("http://cdndownload.openluat.com/rt-thread/airm2m/air105/bootloader.bin", pack_path+'/bootloader.bin')

    if not os.path.exists(pack_path+'/soc_download.exe'):
        urllib.urlretrieve("http://cdndownload.openluat.com/rt-thread/airm2m/air105/soc_download.exe", pack_path+'/soc_download.exe')

    shutil.copy(out_path+bin_file_name, pack_path+'/'+bin_file_name)
#    shutil.make_archive(out_file, 'zip', root_dir=pack_path)
    zip=zipfile.ZipFile(out_file+".zip", "w", compression=zipfile.ZIP_STORED)
    if os.path.isdir(pack_path):
        for (root, dirs, files) in os.walk(pack_path):
            for file in files:
                filePath = os.path.join(root, file)
                zip.write(filePath, file)
    zip.close()
    os.remove(pack_path+'/'+bin_file_name)
    os.rename(out_file+'.zip',out_file+'.soc')
    print('end')
