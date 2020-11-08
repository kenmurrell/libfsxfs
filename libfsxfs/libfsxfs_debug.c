/*
 * Debug functions
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
#include <memory.h>
#include <types.h>

#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the file mode
 */
void libfsxfs_debug_print_file_mode(
      uint16_t file_mode )
{
	if( ( file_mode & 0x0007 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess other: " );

		if( ( file_mode & 0x0004 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0002 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0001 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x0038 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess group: " );

		if( ( file_mode & 0x0020 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0010 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0008 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x01c0 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess user: " );

		if( ( file_mode & 0x0100 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0080 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0040 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\tSticky bit (S_ISTXT)" );
	}
	if( ( file_mode & 0x0400 ) != 0 )
	{
		libcnotify_printf(
		 "\tSet group identifer (GID) on execution (S_ISGID)\n" );
	}
	if( ( file_mode & 0x0800 ) != 0 )
	{
		libcnotify_printf(
		 "\tSet user identifer (UID) on execution (S_ISUID)\n" );
	}
	switch( file_mode & 0xf000 )
	{
		case 0x1000:
			libcnotify_printf(
			 "\tNamed pipe (FIFO) (S_IFIFO)\n" );
			break;

		case 0x2000:
			libcnotify_printf(
			 "\tCharacter device (S_IFCHR)\n" );
			break;

		case 0x4000:
			libcnotify_printf(
			 "\tDirectory (S_IFDIR)\n" );
			break;

		case 0x6000:
			libcnotify_printf(
			 "\tBlock device (S_IFBLK)\n" );
			break;

		case 0x8000:
			libcnotify_printf(
			 "\tRegular file (S_IFREG)\n" );
			break;

		case 0xa000:
			libcnotify_printf(
			 "\tSymbolic link (S_IFLNK)\n" );
			break;

		case 0xc000:
			libcnotify_printf(
			 "\tSocket (S_IFSOCK)\n" );
			break;

		default:
			break;
	}
	libcnotify_printf(
	 "\n" );
}

/* Prints the fork type
 */
const char *libfsxfs_debug_print_fork_type(
             uint8_t fork_type )
{
	switch( fork_type )
	{
		case 0:
			return( "XFS_DINODE_FMT_DEV" );

		case 1:
			return( "XFS_DINODE_FMT_LOCAL" );

		case 2:
			return( "XFS_DINODE_FMT_EXTENTS" );

		case 3:
			return( "XFS_DINODE_FMT_BTREE" );

		case 4:
			return( "XFS_DINODE_FMT_UUID" );

		case 5:
			return( "XFS_DINODE_FMT_RMAP" );
	}
	return( "Unknown" );
}

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid = NULL;
	static char *function       = "libfsxfs_debug_print_guid_value";

	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

