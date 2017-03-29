// -*- c++ -*- Coloration Syntaxique pour Emacs

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const uint32_t N>
void Set<T,N>::append(T const& elt)
{
  // Next element
  m_subindex = MODULO(m_subindex + 1U, M);
  m_index += !m_subindex;
  ++m_last;

  // Reserve a new block of elements if all current blocks are
  // occupied.
  if (m_index >= IContainer<T,N>::m_allocated_blocks)
    {
      IContainer<T,N>::reserveBlocks(1U);
    }

  // Insert element and add the 'Occupied' flag
  IContainer<T,N>::m_blocks[m_index]->m_block[m_subindex] = elt;
  SET_OCCUPIED(m_index, m_subindex);
  ++IContainer<T,N>::m_stored_elements;
}

// **************************************************************
//! \param nth the n'th element (index) of the IContainer we want
//! to remove.
// **************************************************************
template<typename T, const uint32_t N>
void Set<T,N>::remove(const uint32_t nth)
{
  if (outofbound(nth))
    return ;

  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  if (IS_OCCUPIED(id, sid))
    {
      // Replace the nth 'th element by the last inserted element
      IContainer<T,N>::m_blocks[id]->m_block[sid] = (uint32_t) -1;
      removeLast();
    }
}

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N>
void Set<T,N>::removeLast()
{
  // Empty the last inserted element
  CLEAR_OCCUPIED(m_index, m_subindex);

  // Restore index
  if ((0 == m_last) || (((uint32_t) -1) == m_last))
    {
      m_index = m_subindex = m_last = (uint32_t) -1; // FIXME mettre macro
    }
  else
    {
      --m_last;
      m_index = m_last / M;
      m_subindex = MODULO(m_last, M);
    }

  --IContainer<T,N>::m_stored_elements;
}

// **************************************************************
//! \param index1 index of the 1st element to swap.
//! \param index2 index of the 2nd element to swap.
// **************************************************************
template<typename T, const uint32_t N>
void Set<T,N>::swap(const uint32_t index1, const uint32_t index2)
{
  // Do not swapt itself
  if (index1 == index2)
    return ;
  if (outofbound(index1))
    return ;
  if (outofbound(index2))
    return ;

  const uint32_t id1 = index1 / M;
  const uint32_t sid1 = MODULO(index1, M);
  const uint32_t id2 = index2 / M;
  const uint32_t sid2 = MODULO(index2, M);

  T elt = IContainer<T,N>::m_blocks[id2]->m_block[sid2];
  IContainer<T,N>::m_blocks[id2]->m_block[sid2] = IContainer<T,N>::m_blocks[id1]->m_block[sid1];
  IContainer<T,N>::m_blocks[id1]->m_block[sid1] = elt;

  // Note: does not need swap occupied bits because holes are not
  // possible.
}