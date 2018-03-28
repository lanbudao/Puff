#ifndef MSAV_GLOBAL_H
#define MSAV_GLOBAL_H

#ifdef BUILD_MSAV_STATIC
#define MS_AV_EXPORT
#else
#if defined(BUILD_MSAV_LIB)
#  undef MS_AV_EXPORT
#  define MS_AV_EXPORT MS_DECL_EXPORT
#else
#  undef MS_AV_EXPORT
#  define MS_AV_EXPORT MS_DECL_IMPORT
#endif
#endif
#define MS_AV_PRIVATE_EXPORT MS_AV_EXPORT

#endif //MSAV_GLOBAL_H
