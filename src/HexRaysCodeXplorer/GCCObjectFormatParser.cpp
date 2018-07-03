#include "GCCObjectFormatParser.h"
#include "Common.h"
#include "entry.hpp"
#include "Debug.h"
#include "demangle.hpp"
#include "name.hpp"
#include "offset.hpp"
#include "nalt.hpp"
#include "bytes.hpp"
#include "Utility.h"
#include "stddef.h"
#include "GCCVtableInfo.h"
#include "GCCTypeInfo.h"


#define vmi_class_type_info_name "_ZTVN10__cxxabiv121__vmi_class_type_infoE"
#define class_type_info_name "_ZTVN10__cxxabiv117__class_type_infoE"
#define si_class_type_info_name "_ZTVN10__cxxabiv120__si_class_type_infoE"


extern std::map<ea_t, VTBL_info_t> rtti_vftables;

std::map<ea_t, GCCVtableInfo *>g_KnownVtables;
std::map<ea_t, GCCTypeInfo *>g_KnownTypes;

std::set<ea_t> g_rtti_set;
std::set<ea_t> g_vtable_set;

ea_t class_type_info_vtbl = -1;
ea_t si_class_type_info_vtbl = -1;
ea_t vmi_class_type_info_vtbl = -1;



GCCObjectFormatParser::GCCObjectFormatParser()
{
}


GCCObjectFormatParser::~GCCObjectFormatParser()
{
}

void initializeRttiInfo() {
	uint num_import_modules = get_import_module_qty();
	size_t nlist_size = get_nlist_size();
	for (size_t i = 0; i < nlist_size; i++) {
		logmsg(INFO, "%s\n", get_nlist_name(i));
		if (!strcmp(get_nlist_name(i), "__ZTVN10__cxxabiv120__si_class_type_infoE") || !strcmp(get_nlist_name(i), "__cxxabiv1::__si_class_type_info::vtable")) {
			logmsg(INFO, "%s\n", get_nlist_name(i));
			ea_t ea = get_nlist_ea(i);
			si_class_type_info_vtbl = ea;
			// set_name(ea, "__cxxabiv1::__si_class_type_info::vtable", SN_NOWARN);
		}
		if (!strcmp(get_nlist_name(i), "__ZTVN10__cxxabiv117__class_type_infoE") || !strcmp(get_nlist_name(i), "__cxxabiv1::__class_type_info::vtable")) {
			logmsg(INFO, "%s\n", get_nlist_name(i));
			ea_t ea = get_nlist_ea(i);
			class_type_info_vtbl = ea;
			// set_name(ea, "__cxxabiv1::__class_type_info::vtable", SN_NOWARN);;
		}
	}
}

void GCCObjectFormatParser::getRttiInfo()
{
	qstring buffer;
	const size_t count = get_entry_qty();

	uint num_import_modules = get_import_module_qty();
	size_t nlist_size = get_nlist_size();
	for (size_t i = 0; i < nlist_size; i++) {
		logmsg(INFO, "%s\n", get_nlist_name(i));
		if (!strcmp(get_nlist_name(i), "__ZTVN10__cxxabiv120__si_class_type_infoE") || !strcmp(get_nlist_name(i), "__cxxabiv1::__si_class_type_info::vtable")) {
			logmsg(INFO, "%s\n", get_nlist_name(i));
			ea_t ea = get_nlist_ea(i);
			si_class_type_info_vtbl = ea;
			set_name(ea, "__cxxabiv1::__si_class_type_info::vtable", SN_NOWARN);
		}
		if (!strcmp(get_nlist_name(i), "__ZTVN10__cxxabiv117__class_type_infoE") || !strcmp(get_nlist_name(i), "__cxxabiv1::__class_type_info::vtable")) {
			logmsg(INFO, "%s\n", get_nlist_name(i));
			ea_t ea = get_nlist_ea(i);
			class_type_info_vtbl = ea;
			set_name(ea, "__cxxabiv1::__class_type_info::vtable", SN_NOWARN);;
		}
	}

	// First collect info about __cxxabiv1:: vtables
	for (int i = 0; i < count; ++i) {
		uval_t ordinal = get_entry_ordinal(i);
		get_entry_name(&buffer, ordinal);
		ea_t ea = get_entry(ordinal);
		ea += offsetof(GCC_RTTI::__vtable_info, origin);

		if (buffer == class_type_info_name)
		{
			class_type_info_vtbl = ea;
			// set_name(ea, "__cxxabiv1::__class_type_info::vtable", SN_NOWARN);
		}

		if (buffer == si_class_type_info_name)
		{
			si_class_type_info_vtbl = ea;
			// set_name(ea, "__cxxabiv1::__si_class_type_info::vtable", SN_NOWARN);
		}

		if (buffer == vmi_class_type_info_name)
		{
			vmi_class_type_info_vtbl = ea;
			set_name(ea, "__cxxabiv1::__vmi_class_type_info::vtable", SN_NOWARN);
		}
	}
	// now we can scan  segments for vtables.
	int segCount = get_segm_qty();
	qstring seg_name_buffer;
	for (int i = 0; i < segCount; i++)
	{
		if (segment_t *seg = getnseg(i))
		{
			// logmsg(INFO, "%s\n", seg->name);
			if (seg->type == SEG_DATA)
			{
				get_segm_name(&seg_name_buffer, seg, 0);
				if (seg_name_buffer == "__data")
					scanSeg4Vftables(seg);
			}
		}
	}
}

void search_rtti_set(segment_t *seg, std::set<ea_t> *rtti_set) {
	ea_t startEA = seg->start_ea;
	ea_t endEA = seg->end_ea;

	for (ea_t rtti_ptr = startEA; rtti_ptr < endEA; rtti_ptr += sizeof(ea_t)) {
		ea_t tmp;
		if (!get_bytes(&tmp, sizeof(ea_t), rtti_ptr))
			return;

		//std::vector<ea_t>::iterator iter = std::find(rtti_set.begin(), rtti_set.end(), tmp);
		//if (iter != rtti_set.end()) {
		//	rtti_set.push_back(ptr);
		//}

		if (tmp == class_type_info_vtbl || tmp == si_class_type_info_vtbl || tmp == vmi_class_type_info_vtbl) {
			rtti_set->insert(rtti_ptr);
		}
	}

}

void search_vtable_set(segment_t *seg, std::set<ea_t> *rtti_set, std::set<ea_t> *vtable_set) {
	ea_t startEA = seg->start_ea;
	ea_t endEA = seg->end_ea;
	for (ea_t vtable_ptr = startEA; vtable_ptr < endEA; vtable_ptr += sizeof(ea_t)) {
		ea_t tmp;
		if (!get_bytes(&tmp, sizeof(ea_t), vtable_ptr))
			return;

		// check vtable entry
		if (rtti_set->find(tmp) != rtti_set->end()) {
			vtable_set->insert(vtable_ptr - sizeof(ea_t));
		}
	}
}

void GCCObjectFormatParser::scanSeg4Vftables(segment_t *seg)
{

	std::set<ea_t>::iterator it = g_vtable_set.begin();
	for (it; it != g_vtable_set.end(); it++) {
		GCCVtableInfo * info = GCCVtableInfo::parseVtableInfo(ptr);
		if (info)
		{
			VTBL_info_t vtbl_info;
			vtbl_info.ea_begin = info->vtbl_start;
			vtbl_info.ea_end = info->ea_end;
			vtbl_info.vtbl_name = info->typeName;
			vtbl_info.methods = info->vtables[0].methodsCount;
			rtti_vftables[ptr + offsetof(GCC_RTTI::__vtable_info, origin)] = vtbl_info;
			ptr = info->ea_end;
		}
		else {

			GCCTypeInfo *typeInfo = GCCTypeInfo::parseTypeInfo(ptr);
			if (typeInfo)
			{
				;
			}
		}
	}
	return;
}

void GCCObjectFormatParser::clearInfo()
{
	g_KnownVtables.clear();
	g_KnownTypes.clear();
}