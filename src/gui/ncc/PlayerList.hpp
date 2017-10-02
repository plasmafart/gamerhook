/*
 * PlayerList.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: nullifiedcat
 */

#ifndef GUI_NCC_PLAYERLIST_HPP_
#define GUI_NCC_PLAYERLIST_HPP_

#include "../CBaseContainer.h"

namespace menu { namespace ncc {

constexpr int size_table[] = { 32, 32, 80, 160, 80, 80, 32, 32, 32 };
constexpr int size_table_width() {
	int accum = 1;
	for (int i = 0; i < sizeof(size_table) / sizeof(int); i++) {
		accum += (size_table[i] + 1);
	}
	return accum;
}

class PlayerList : public CBaseContainer {
public:
	PlayerList();

	virtual void Draw(int x, int y) override;
	virtual void Update() override;
	virtual void OnKeyPress(ButtonCode_t key, bool repeat) override;
	virtual void MoveChildren() override;
	inline virtual void SortByZIndex() override {};
};

}}

#endif /* GUI_NCC_PLAYERLIST_HPP_ */
