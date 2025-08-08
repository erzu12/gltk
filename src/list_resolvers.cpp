// #include "list_resolvers.h"
// #include <iostream>
// #include <vector>
//
// namespace gltk {
//
// BoundingBox ListStrechResolver::resolve(std::vector<ChildData> children, Vec2 resolvedSize) {
//     Vec2 currentPosition = getListStartPossition();
//     float totalAbsoluteHeight = 0;
//     float totalRelativeHeight = 0;
//     std::vector<Vec2> minChildSizes(children.size());
//     std::vector<bool> childIsAbsolute(children.size());
//     std::vector<Vec2> childSizes(children.size());
//
//     for (int i = 0; i < children.size(); i++) {
//         auto child = children[i];
//         Vec2 childSize = child.size->resolve(resolvedSize);
//         Sizing childSizing = getListDirectionSizing(child.horizontalSizing, child.verticalSizing);
//         if (Sizing::Shrink == childSizing) {
//             throw layout_exception("Cannot have a child with Sizing Shrink in a ListStretch layout");
//         }
//         minChildSizes[i] = getChildMinSize(&child);
//         childIsAbsolute[i] = getListDirectionMessure(child.size)->isAbsolute();
//         if (Sizing::Fit == childSizing) {
//             childSize = minChildSizes[i];
//             childIsAbsolute[i] = true;
//         }
//         if (childIsAbsolute[i]) {
//             totalAbsoluteHeight += getListDirectionValue(childSize);
//             childSizes[i] = childSize;
//         } else {
//             totalRelativeHeight += getListDirectionValue(childSize);
//         }
//     }
//
//     float remainingHeight = getListDirectionValue(resolvedSize) - totalAbsoluteHeight;
//
//     int childIndex = 0;
//     while (childIndex < children.size()) {
//         if (childIsAbsolute[childIndex]) {
//             childIndex++;
//             continue;
//         }
//         auto child = children[childIndex];
//         Vec2 childSize = child.size->resolve(resolvedSize);
//         Vec2 originalChildSize = childSize;
//         getListDirectionValue(childSize) = remainingHeight * getListDirectionValue(childSize) / totalRelativeHeight;
//         if (getListDirectionValue(childSize) < getListDirectionValue(minChildSizes[childIndex])) {
//             getListDirectionValue(childSize) = getListDirectionValue(minChildSizes[childIndex]);
//             totalRelativeHeight -= getListDirectionValue(originalChildSize);
//             totalAbsoluteHeight += getListDirectionValue(childSize);
//             remainingHeight = getListDirectionValue(resolvedSize) - totalAbsoluteHeight;
//             childIsAbsolute[childIndex] = true;
//             childSizes[childIndex] = childSize;
//             childIndex = 0;
//         } else {
//             childSizes[childIndex] = childSize;
//             childIndex++;
//         }
//     }
//
//     BoundingBox childBounds;
//     for (int i = 0; i < children.size(); i++) {
//         auto child = children[i];
//         Vec2 childPosition = currentPosition;
//         Vec2 childSize = childSizes[i];
//         if (ListDirection::Up == listDirection) {
//             childPosition.x -= resolvedSize.x;
//             childPosition.y -= childSize.y;
//         }
//         if (ListDirection::Left == listDirection) {
//             childPosition.y -= resolvedSize.y;
//             childPosition.x -= childSize.x;
//         }
//         BoundingBox retBound = child.resolveTransform(getListParentSize(childSize), childPosition, true,
//         listDirection); childBounds.add(retBound); adjustCurrentPosition(childSize, currentPosition);
//     }
//     return childBounds;
// }
//
// void ListStrechResolver::adjustCurrentPosition(Vec2 childSize, Vec2 &currentPosition) {
//     switch (listDirection) {
//     case ListDirection::Down:
//         currentPosition.y += childSize.y;
//         break;
//     case ListDirection::Right:
//         currentPosition.x += childSize.x;
//         break;
//     case ListDirection::Left:
//         currentPosition.x -= childSize.x;
//         break;
//     case ListDirection::Up:
//         currentPosition.y -= childSize.y;
//         break;
//     }
// }
//
// Vec2 ListStrechResolver::getListStartPossition() {
//     if (ListDirection::Down == listDirection || ListDirection::Right == listDirection) {
//         return resolvedPosition;
//     } else {
//         Vec2 currentPosition = resolvedPosition + resolvedSize;
//         return currentPosition;
//     }
// }
//
// Vec2 ListStrechResolver::getListParentSize(Vec2 childSize) {
//     if (ListDirection::Down == listDirection || ListDirection::Up == listDirection) {
//         return Vec2(resolvedSize.x, childSize.y);
//     } else {
//         return Vec2(childSize.x, resolvedSize.y);
//     }
// }
//
// IMessure *ListStrechResolver::getListDirectionMessure(MessureVec2 *messure) {
//     if (listDirection == ListDirection::Down || listDirection == ListDirection::Up) {
//         return messure->y.get();
//     } else {
//         return messure->x.get();
//     }
// }
//
// float &ListStrechResolver::getListDirectionValue(Vec2 &value) {
//     if (listDirection == ListDirection::Down || listDirection == ListDirection::Up) {
//         return value.y;
//     } else {
//         return value.x;
//     }
// }
//
// Vec2 ListStrechResolver::getChildMinSize(ChildData *child) {
//     if (getListDirectionSizing(child->horizontalSizing, child->verticalSizing) == Sizing::Fixed ||
//         getListDirectionSizing(child->horizontalSizing, child->verticalSizing) == Sizing::Shrink) {
//         return Vec2(0, 0);
//     }
//     BoundingBox childBounds;
//     childBounds = child->resolveTransform(getListParentSize(Vec2()), Vec2(0, 0), true, ListDirection::Down);
//     Vec2 resolvedChildSize = childBounds.size();
//     if (getListDirectionValue(resolvedChildSize) < 0.001f) {
//         std::cout << "WARNING: child layout with Sizing: Expand or Fit of ListStretch layout has no size. Make sure
//         at "
//                      "least one child has an absolute size. This can cause overlap of children"
//                   << std::endl;
//     }
//     return childBounds.size();
// }
//
// } // namespace gltk
