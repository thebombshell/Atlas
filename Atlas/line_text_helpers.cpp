
// line_text_helpers.cpp

#include "line_text_helpers.hpp"

using namespace pantheon;
using namespace atlas;

bool g_isInit{ false };
typedef std::map<char, std::vector<LineRendererVertex>> CharVertMap;
CharVertMap* g_characterMap{ };

void init() {
	
	std::vector<std::pair<const char, std::vector<LineRendererVertex>>> characters {
		{ 'a',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'b',
		{
			{ 3.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'c',
		{
			{ 4.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'd',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'e',
		{
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'f',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'g',
		{
			{ 2.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'h',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'i',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'j',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'k',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 3.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'l',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'm',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 2.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'n',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'o',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ LineRendererVertex::separator() }
		} }
		, { 'p',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'q',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ LineRendererVertex::separator() },
			{ 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'r',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 's',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 't',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 2.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'u',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'v',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'w',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'x',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'y',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 2.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { 'z',
		{
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '0',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ LineRendererVertex::separator() },
			{ 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '1',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '2',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '3',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '4',
		{
			{ 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '5',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '6',
		{
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '7',
		{
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '8',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ LineRendererVertex::separator() },
			{ 1.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '9',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '.',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.01f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '&',
		{
			{ 1.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 2.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.01f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.01f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
		, { '!',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.01f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
		} }
		, { '?',
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.01f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() },
			{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{ LineRendererVertex::separator() }
		} }
	};
	g_characterMap = new CharVertMap( characters.begin(), characters.end() );
	g_isInit = true;
}

std::vector<LineRendererVertex> atlas::charToLines( char t_char, int t_offset ) {

	if ( !g_isInit ) {

		init();
	}

	float offset = t_offset * 6.0f;
	auto pair = g_characterMap->find( t_char );

	if ( pair != g_characterMap->end() ) {
		std::vector<LineRendererVertex> vertices{ pair->second };

		for ( auto& vertex : vertices ) {

			if ( !vertex.isSeperator() ) {

				vertex.position[0] += offset;
			}
		}
		return vertices;
	}
	return{ };
}

std::vector<LineRendererVertex> atlas::charToLines( char t_char, int t_offset, const glm::vec3& t_pos ) {

	if ( !g_isInit ) {

		init();
	}

	float offset = t_offset * 6.0f;
	auto pair = g_characterMap->find( t_char );

	if ( pair != g_characterMap->end() ) {
		std::vector<LineRendererVertex> vertices{ pair->second };

		for ( auto& vertex : vertices ) {

			if ( !vertex.isSeperator() ) {

				vertex.position[0] += offset + t_pos[0];
				vertex.position[1] += t_pos[1];
				vertex.position[2] += t_pos[2];
			}
		}
		return vertices;
	}
	return{ };
}

std::vector<LineRendererVertex> atlas::charToLines( char t_char, int t_offset, const glm::vec3& t_pos, const glm::vec3& t_col ) {

	if ( !g_isInit ) {

		init();
	}

	float offset = t_offset * 6.0f;
	auto pair = g_characterMap->find( t_char );

	if ( pair != g_characterMap->end() ) {
		std::vector<LineRendererVertex> vertices{ pair->second };

		for ( auto& vertex : vertices ) {

			if ( !vertex.isSeperator() ) {

				vertex.position[0] += offset + t_pos[0];
				vertex.position[1] += t_pos[1];
				vertex.position[2] += t_pos[2];
				vertex.colour[0] = t_col[0];
				vertex.colour[1] = t_col[1];
				vertex.colour[2] = t_col[2];
			}
		}
		return vertices;
	}
	return{ };
}

LineRendererMessage atlas::stringToLines( std::string t_string ) {

	LineRendererMessage message{ };
	int offset{ 0 };
	for ( auto character : t_string ) {

		auto vertices = charToLines( character, offset );
		if ( vertices.size() > 0 ) {

			message.pushVertices( vertices.begin(), vertices.end() );
		}
		++offset;
	}
	return message;
}

LineRendererMessage atlas::stringToLines( std::string t_string, const glm::vec3& t_position ) {

	LineRendererMessage message{ };
	int offset{ 0 };
	for ( auto character : t_string ) {

		auto vertices = charToLines( character, offset, t_position );
		if ( vertices.size() > 0 ) {

			message.pushVertices( vertices.begin(), vertices.end() );
		}
		++offset;
	}
	return message;
}

LineRendererMessage atlas::stringToLines( std::string t_string, const glm::vec3& t_position, const glm::vec3& t_colour ) {

	LineRendererMessage message{ };
	int offset{ 0 };
	for ( auto character : t_string ) {

		auto vertices = charToLines( character, offset, t_position, t_colour );
		if ( vertices.size() > 0 ) {

			message.pushVertices( vertices.begin(), vertices.end() );
		}
		++offset;
	}
	return message;
}