/*
 * Inode functions
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsxfs_debug.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_inode.h"

/* Creates a inode
 * Make sure the value inode is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_initialize(
     libfsxfs_inode_t **inode,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_initialize";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode value already set.",
		 function );

		return( -1 );
	}
	*inode = memory_allocate_structure(
	          libfsxfs_inode_t );

	if( *inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode,
	     0,
	     sizeof( libfsxfs_inode_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *inode != NULL )
	{
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( -1 );
}

/* Frees a inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_free(
     libfsxfs_inode_t **inode,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_free";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( 1 );
}

/* Reads the inode data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_read_data(
     libfsxfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function  = "libfsxfs_inode_read_data";
	size_t inode_data_size = 0;
	uint32_t value_32bit   = 0;
	uint8_t format_version = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit   = 0;
	uint16_t value_16bit   = 0;
#endif

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsxfs_inode_v1_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	format_version = ( (fsxfs_inode_v1_t *) data )->format_version;

	if( format_version == 3 )
	{
		if( data_size < sizeof( fsxfs_inode_v3_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			return( -1 );
		}
		inode_data_size = sizeof( fsxfs_inode_v3_t );
	}
	else
	{
		inode_data_size = sizeof( fsxfs_inode_v1_t );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: inode data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 inode_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	    ( (fsxfs_inode_v1_t *) data )->signature,
	     "IN",
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->file_mode,
	 inode->file_mode );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->owner_identifier,
	 inode->owner_identifier );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->group_identifier,
	 inode->group_identifier );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->access_time,
	 inode->access_time );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->access_time_nano_seconds,
	 value_32bit );

	inode->access_time *= 1000000000;
	inode->access_time += value_32bit;

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->modification_time,
	 inode->modification_time );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->modification_time_nano_seconds,
	 value_32bit );

	inode->modification_time *= 1000000000;
	inode->modification_time += value_32bit;

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->inode_change_time,
	 inode->inode_change_time );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->inode_change_time_nano_seconds,
	 value_32bit );

	inode->inode_change_time *= 1000000000;
	inode->inode_change_time += value_32bit;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->signature[ 0 ],
		 ( (fsxfs_inode_v1_t *) data )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: file mode\t\t\t\t\t: 0o07%" PRIo16 " (0x%04" PRIx16 ")\n",
		 function,
		 inode->file_mode,
		 inode->file_mode );
		libfsxfs_debug_print_file_mode(
		 inode->file_mode );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu8 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: fork type\t\t\t\t\t: %" PRIu8 " (%s)\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->fork_type,
		 libfsxfs_debug_print_fork_type(
		  ( (fsxfs_inode_v1_t *) data )->fork_type ) );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->number_of_links,
		 value_16bit );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: number of links\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );
		}
		else
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		libcnotify_printf(
		 "%s: owner identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 inode->owner_identifier );

		libcnotify_printf(
		 "%s: group identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 inode->group_identifier );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 ( (fsxfs_inode_v1_t *) data )->unknown2,
			 14,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v2_t *) data )->number_of_links,
			 value_32bit );
			libcnotify_printf(
			 "%s: number of links\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_inode_v2_t *) data )->project_identifier,
			 value_16bit );
			libcnotify_printf(
			 "%s: project identifier\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v2_t *) data )->unknown2,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->flush_counter,
		 value_16bit );
		libcnotify_printf(
		 "%s: flush counter\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->access_time,
		 value_32bit );
		libcnotify_printf(
		 "%s: access time\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->access_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: access time nano seconds\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->modification_time,
		 value_32bit );
		libcnotify_printf(
		 "%s: modification time\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->modification_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: modification time nano seconds\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->inode_change_time,
		 value_32bit );
		libcnotify_printf(
		 "%s: inode change time\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->inode_change_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: inode change time nano seconds\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->extent_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: extent size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->number_of_data_extents,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of data extents\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->number_of_attributes_extents,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of attributes extents\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown3\t\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->unknown3 );

		libcnotify_printf(
		 "%s: unknown4\t\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->unknown4 );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->unknown5,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->unknown6,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->inode_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: inode flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->generation_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: generation number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( format_version != 1 )
	 && ( format_version != 2 )
	 && ( format_version != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu32 ".",
		 function,
		 format_version );

		return( -1 );
	}
	if( format_version == 3 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->unknown7,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown7\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->change_count,
			 value_64bit );
			libcnotify_printf(
			 "%s: change count\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->log_sequence_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: log sequence number\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->extended_inode_flags,
			 value_64bit );
			libcnotify_printf(
			 "%s: extended inode flags\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->cow_extent_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: copy-on-write extent size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (fsxfs_inode_v3_t *) data )->unknown8,
			 12,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->creation_time,
			 value_32bit );
			libcnotify_printf(
			 "%s: creation time\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->creation_time_nano_seconds,
			 value_32bit );
			libcnotify_printf(
			 "%s: creation time nano seconds\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->inode_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: inode number\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			if( libfsxfs_debug_print_guid_value(
			     function,
			     "inode type identifier\t\t\t\t",
			     ( (fsxfs_inode_v3_t *) data )->inode_type_identifier,
			     16,
			     LIBFGUID_ENDIAN_BIG,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the inode from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_read_file_io_handle(
     libfsxfs_inode_t *inode,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size_t inode_size,
     libcerror_error_t **error )
{
	uint8_t *inode_data   = NULL;
	static char *function = "libfsxfs_inode_read_file_io_handle";
	ssize_t read_count    = 0;

	if( ( inode_size != 256 )
	 && ( inode_size != 512 )
	 && ( inode_size != 1024 )
	 && ( inode_size != 2048 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported inode size: %" PRIzd ".",
		 function,
		 inode_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading inode at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek inode offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	inode_data = (uint8_t *) memory_allocate(
	                          sizeof( uint8_t ) * inode_size );

	if( inode_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              inode_data,
	              inode_size,
	              error );

	if( read_count != (ssize_t) inode_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libfsxfs_inode_read_data(
	     inode,
	     inode_data,
	     inode_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	memory_free(
	 inode_data );

	return( 1 );

on_error:
	if( inode_data != NULL )
	{
		memory_free(
		 inode_data );
	}
	return( 1 );
}

