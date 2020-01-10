/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "dll.h"

#ifdef STEAM_WIN32
// Nothing to be done here
#else

#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <unistd.h>
#include <utime.h>

STEAMAPI_API FILE *__wrap_freopen(const char *pathname, const char *mode, FILE *stream)
{
  return freopen(pathname, mode, stream);
}

STEAMAPI_API FILE *__wrap_fopen(const char *filename, const char *mode)
{
  return fopen(filename, mode);
}

STEAMAPI_API FILE *__wrap_fopen64(const char *filename, const char *mode)
{
  return fopen64(filename, mode);
}

STEAMAPI_API int __wrap_open(const char *pathname, int flags, mode_t mode)
{
  return open(pathname, flags, mode);
}

STEAMAPI_API int __wrap_open64(const char *pathname, int flags, mode_t mode)
{
  return open64(pathname, flags, mode);
}

STEAMAPI_API int __wrap_access(const char *pathname, int mode)
{
  return access(pathname, mode);
}

STEAMAPI_API int __wrap___xstat(int ver, const char * path, struct stat * stat_buf)
{
  return __xstat(ver, path, stat_buf);
}

STEAMAPI_API int __wrap_stat(const char * path, struct stat * stat_buf)
{
  return __wrap___xstat(3, path, stat_buf);
}

STEAMAPI_API int __wrap___lxstat(int ver, const char * path, struct stat * stat_buf)
{
  return __lxstat(ver, path, stat_buf);
}

STEAMAPI_API int __wrap_lstat(const char * path, struct stat * stat_buf)
{
  return __wrap___lxstat(3, path, stat_buf);
}

STEAMAPI_API int __wrap_scandir(const char *dir, struct dirent ***namelist, int (*sel)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **))
{
  return scandir(dir, namelist, sel, compar);
}

STEAMAPI_API int __wrap_scandir64(const char *dir, struct dirent64 ***namelist, int (*sel)(const struct dirent64 *), int (*compar)(const struct dirent64 **, const struct dirent64 **))
{
  return scandir64(dir, namelist, sel, compar);
}

STEAMAPI_API DIR *__wrap_opendir(const char *name)
{
  return opendir(name);
}

STEAMAPI_API int __wrap___xstat64(int vers, const char *name, struct stat64 *buf)
{
  return __xstat64(vers, name, buf);
}

STEAMAPI_API int __wrap___lxstat64(int vers, const char *name, struct stat64 *buf)
{
  return __lxstat64(vers, name, buf);
}

STEAMAPI_API int __wrap_statvfs(const char *path, struct statvfs *buf)
{
  return statvfs(path, buf);
}

STEAMAPI_API int __wrap_statvfs64(const char *path, struct statvfs64 *buf)
{
  return statvfs64(path, buf);
}

STEAMAPI_API int __wrap_chmod(const char *pathname, mode_t mode)
{
  return chmod(pathname, mode);
}

STEAMAPI_API int __wrap_chown(const char *path, uid_t owner, gid_t group)
{
  return chown(path, owner, group);
}

STEAMAPI_API int __wrap_lchown(const char *path, uid_t owner, gid_t group)
{
  return lchown(path, owner, group);
}

STEAMAPI_API int __wrap_symlink(const char *path1, const char *path2)
{
  return symlink(path1, path2);
}

STEAMAPI_API int __wrap_link(const char *path1, const char *path2)
{
  return link(path1, path2);
}

STEAMAPI_API int __wrap_mknod(const char *path, mode_t mode, dev_t dev)
{
  return __xmknod(1, path, mode, &dev);
}

STEAMAPI_API int __wrap_mount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
  return mount(source, target, filesystemtype, mountflags, data);
}

STEAMAPI_API int __wrap_unlink(const char *path)
{
  return unlink(path);
}

STEAMAPI_API int __wrap_mkfifo(const char *path, mode_t mode)
{
  return mkfifo(path, mode);
}

STEAMAPI_API int __wrap_rename(const char *old_name, const char *new_name)
{
  return rename(old_name, new_name);
}

STEAMAPI_API int __wrap_utime(const char *path, const struct utimbuf *times)
{
  return utime(path, times);
}

STEAMAPI_API int __wrap_utimes(const char *path, const struct timeval times[2])
{
  return utimes(path, times);
}

STEAMAPI_API int __wrap_mkdir(const char *path, mode_t mode)
{
  return mkdir(path, mode);
}

STEAMAPI_API int __wrap_rmdir(const char *path)
{
  return rmdir(path);
}

STEAMAPI_API void * __wrap_dlopen(const char *file, int mode)
{
  return dlopen(file, mode);
}

STEAMAPI_API void * __wrap_dlmopen(Lmid_t lmid, const char *filename, int flags)
{
  return dlmopen(lmid, filename, flags);
}

#endif