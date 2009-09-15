/* radare - LGPL - Copyright 2009 nibble<.ds@gmail.com> */

[CCode (cheader_filename="r_bin.h", cprefix="r_bin_", lower_case_cprefix="r_bin_")]
namespace Radare {
	[Compact]
	[CCode (cname="struct r_bin_t", free_function="r_bin_free", cprefix="r_bin_")]
	public class Bin {
		public const string file;
		public int fd;

		public Bin();
		public bool open(string file, bool rw, string? plugin_name = null);

		public int init(string file, int rw);
		public int close();
		public uint64 get_baddr();
		public Entrypoint* get_entry();
		public Section* get_sections();

		public Radare.List<Bin.Symbol*> symbols;

		public Symbol* get_symbols();
		public Import* get_imports();

		public Info* get_info();
		public uint64 get_section_offset(string name);
		public uint64 get_section_rva(string name);
		public uint32 get_section_size(string name);
		public uint64 resize_section(string name, uint64 size);
	
		[CCode (cname="struct r_bin_entry_t")]
		public struct Entrypoint {
			public uint64 rva;
			public uint64 offset;
		}

		[CCode (cname="struct r_bin_section_t")]
		public struct Section {
			public string name;
			public int32 size;
			public int32 vsize;
			public int64 rva;
			public int64 offset;
			public int32 stringacteristics;
			public bool last;
		}

		[CCode (cname="struct r_bin_symbol_t")]
		public struct Symbol {
			public string name;
			public string forwarder;
			public string bind;
			public string type;
			public uint64 rva;
			public uint64 offset;
			public uint32 size;
			public uint32 ordinal;
			public bool last;
		}

		[CCode (cname="struct r_bin_import_t")]
		public struct Import {
			public string name;
			public string bind;
			public string type;
			public uint64 rva;
			public uint64 offset;
			public uint32 ordinal;
			public uint32 hint;
			public bool last;
		}

		[CCode (cname="struct r_bin_info_t")]
		public struct Info {
			public string type;
			public string @class;
			public string rclass;
			public string arch;
			public string machine;
			public string os;
			public string subsystem;
			public int bigendian;
			public uint32 dbg_info;
		}
	}
}
