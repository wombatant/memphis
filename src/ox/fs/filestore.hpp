/*
 * Copyright 2015 - 2017 gtalent2@gmail.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <ox/std/std.hpp>

namespace ox {
namespace fs {

template<typename FsT, typename InodeId>
struct FileStoreHeader {
	public:
		typedef InodeId InodeId_t;
		typedef FsT FsSize_t;
		const static auto VERSION = 4;

	private:
		uint16_t m_version;
		uint16_t m_fsType;
		FsSize_t m_size;
		FsSize_t m_memUsed;
		FsSize_t m_rootInode;

	public:
		void setVersion(uint16_t);
		uint16_t getVersion();

		void setFsType(uint16_t);
		uint16_t getFsType();

		void setSize(FsSize_t);
		FsSize_t getSize();

		void setMemUsed(FsSize_t);
		FsSize_t getMemUsed();

		void setRootInode(FsSize_t);
		FsSize_t getRootInode();
};

template<typename FsSize_t, typename InodeId_t>
void FileStoreHeader<FsSize_t, InodeId_t>::setVersion(uint16_t version) {
	m_version = version;
}

template<typename FsSize_t, typename InodeId_t>
uint16_t FileStoreHeader<FsSize_t, InodeId_t>::getVersion() {
	return m_version;
}

template<typename FsSize_t, typename InodeId_t>
void FileStoreHeader<FsSize_t, InodeId_t>::setFsType(uint16_t fsType) {
	m_fsType = fsType;
}

template<typename FsSize_t, typename InodeId_t>
uint16_t FileStoreHeader<FsSize_t, InodeId_t>::getFsType() {
	return m_fsType;
}

template<typename FsSize_t, typename InodeId_t>
void FileStoreHeader<FsSize_t, InodeId_t>::setSize(FsSize_t size) {
	m_size = size;
}

template<typename FsSize_t, typename InodeId_t>
FsSize_t FileStoreHeader<FsSize_t, InodeId_t>::getSize() {
	return m_size;
}

template<typename FsSize_t, typename InodeId_t>
void FileStoreHeader<FsSize_t, InodeId_t>::setMemUsed(FsSize_t memUsed) {
	m_memUsed = memUsed;
}

template<typename FsSize_t, typename InodeId_t>
FsSize_t FileStoreHeader<FsSize_t, InodeId_t>::getMemUsed() {
	return m_memUsed;
}

template<typename FsSize_t, typename InodeId_t>
void FileStoreHeader<FsSize_t, InodeId_t>::setRootInode(FsSize_t rootInode) {
	m_rootInode = rootInode;
}

template<typename FsSize_t, typename InodeId_t>
FsSize_t FileStoreHeader<FsSize_t, InodeId_t>::getRootInode() {
	return m_rootInode;
}

template<typename Header>
class FileStore {

	public:
		typedef typename Header::InodeId_t InodeId_t;
		typedef typename Header::FsSize_t FsSize_t;
		const static auto VERSION = Header::VERSION;

		struct StatInfo {
			InodeId_t inodeId;
			typename Header::FsSize_t  size;
			uint8_t fileType;
		};

	private:
		struct Inode {
			private:
				// the next Inode in memory
				typename Header::FsSize_t m_prev, m_next;
				typename Header::FsSize_t m_dataLen;

				InodeId_t m_id;
				uint8_t m_fileType;
				typename Header::FsSize_t m_left, m_right;

			public:
				typename Header::FsSize_t size();

				typename Header::FsSize_t getDataLen();

				void setPrev(typename Header::FsSize_t);
				typename Header::FsSize_t getPrev();

				void setNext(typename Header::FsSize_t);
				typename Header::FsSize_t getNext();

				void setId(InodeId_t);
				InodeId_t getId();

				void setFileType(uint8_t);
				uint8_t getFileType();

				void setLeft(typename Header::FsSize_t);
				typename Header::FsSize_t getLeft();

				void setRight(typename Header::FsSize_t);
				typename Header::FsSize_t getRight();

				void setData(void *data, typename Header::FsSize_t size);
				void *getData();
		};

		Header m_header;

	public:
		/**
		 * Dumps this file store's inodes to the given file store.
		 */
		int dumpTo(FileStore<Header> *dest);

		/**
		 * Compacts and resizes the file store to the minimum possible size for
		 * the contents.
		 *
		 * NOTE: This does NOT resize the buffer that this file store refers to!
		 */
		void resize(typename Header::FsSize_t size = 0);

		/**
		 * Writes the given data to a "file" with the given id.
		 * @param id the id of the file
		 * @param data the contents of the file
		 * @param dataLen the number of bytes data points to
		 */
		int write(InodeId_t id, void *data, typename Header::FsSize_t dataLen, uint8_t fileType = 0);

		/**
		 * Removes the inode of the given ID.
		 * @param id the id of the file
		 */
		int remove(InodeId_t id);

		/**
		 * Reads the "file" at the given id. You are responsible for freeing
		 * the data when done with it.
		 * @param id id of the "file"
		 * @param data pointer to the pointer where the data is stored
		 * @param size pointer to a value that will be assigned the size of data
		 * @return 0 if read is a success
		 */
		int read(InodeId_t id, void *data, typename Header::FsSize_t *size);

		/**
		 * Reads the stat information of the inode of the given inode id.
		 * If the returned inode id is 0, then the requested inode was not found.
		 * @param id id of the inode to stat
		 * @return the stat information of the inode of the given inode id
		 */
		StatInfo stat(InodeId_t id);

		/**
		 * Returns the space needed for this data at the given inode address.
		 * @param id the target inode id
		 * @param size the size of the data to insert
		 * @return the space currently available in this file store.
		 */
		typename Header::FsSize_t spaceNeeded(InodeId_t id, typename Header::FsSize_t size);

		/**
		 * Returns the size of the file store.
		 * @return the size of the file store.
		 */
		typename Header::FsSize_t size();

		/**
		 * Returns the space currently available in this file store.
		 * @return the space currently available in this file store.
		 */
		typename Header::FsSize_t available();

		uint16_t fsType();

		uint16_t version();
		
		static uint8_t *format(uint8_t *buffer, typename Header::FsSize_t size, uint16_t fsType = 0);

	private:
		/**
		 * Gets the inode at the given id.
		 * @param root the root node to start comparing on
		 * @param id id of the "file"
		 * @param pathLen number of characters in pathLen
		 * @return the requested Inode, if available
		 */
		Inode *getInode(Inode *root, InodeId_t id);

		/**
		 * Gets the inode at the given id.
		 * @param root the root node to start comparing on
		 * @param id id of the "file"
		 * @param pathLen number of characters in pathLen
		 * @param targetAddr the address of the target inode
		 * @return the requested Inode, if available
		 */
		Inode *getInodeParent(Inode *root, InodeId_t id, typename Header::FsSize_t targetAddr);

		/**
		 * Removes the inode of the given ID.
		 * @param id the id of the file
		 */
		int remove(Inode *root, InodeId_t id);

		/**
		 * Removes the given node from the linked list.
		 * @param node node to remove
		 */
		void dealloc(Inode *node);

		/**
		 * Gets the address of the next available inode, assuming there is a next
		 * available inode.
		 */
		typename Header::FsSize_t nextInodeAddr();

		/**
		 * Gets an address for a new Inode.
		 * @param size the size of the Inode
		 */
		void *alloc(typename Header::FsSize_t size);

		/**
		 * Compacts all of the inodes into a contiguous space, starting at the first inode.
		 */
		void compact();

		/**
		 * Inserts the given insertValue into the tree of the given root.
		 * If the inode already exists, it replaces the old on deletes it.
		 * @return true if the inode was inserted
		 */
		bool insert(Inode *root, Inode *insertValue);

		/**
		 * Gets the FsSize_t associated with the next Inode to be allocated.
		 * @retrun the FsSize_t associated with the next Inode to be allocated
		 */
		typename Header::FsSize_t iterator();

		typename Header::FsSize_t firstInode();

		Inode *lastInode();

		/**
		 * Updates the address of the inode in the tree.
		 */
		void updateInodeAddress(InodeId_t id, typename Header::FsSize_t oldAddr, typename Header::FsSize_t newAddr);

		uint8_t *begin() {
			return (uint8_t*) this;
		}

		uint8_t *end() {
			return begin() + this->m_header.getSize();
		}

		/**
		 * Converts an actual pointer to a FsSize_t.
		 */
		typename Header::FsSize_t ptr(void *ptr);

		/**
		 * Converts a FsSize_t to an actual pointer.
		 */
		template<typename T>
		T ptr(typename Header::FsSize_t ptr) {
			return (T) (begin() + ptr);
		};

};

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::size() {
	return sizeof(Inode) + m_dataLen;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::getDataLen() {
	return this->m_dataLen;
}

template<typename Header>
void FileStore<Header>::Inode::setPrev(typename Header::FsSize_t prev) {
	this->m_prev = prev;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::getPrev() {
	return this->m_prev;
}

template<typename Header>
void FileStore<Header>::Inode::setNext(typename Header::FsSize_t next) {
	this->m_next = next;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::getNext() {
	return this->m_next;
}

template<typename Header>
void FileStore<Header>::Inode::setId(InodeId_t id) {
	this->m_id = id;
}

template<typename Header>
typename Header::InodeId_t FileStore<Header>::Inode::getId() {
	return this->m_id;
}

template<typename Header>
void FileStore<Header>::Inode::setFileType(uint8_t fileType) {
	this->m_fileType = fileType;
}

template<typename Header>
uint8_t FileStore<Header>::Inode::getFileType() {
	return this->m_fileType;
}

template<typename Header>
void FileStore<Header>::Inode::setLeft(typename Header::FsSize_t left) {
	this->m_left = left;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::getLeft() {
	return this->m_left;
}

template<typename Header>
void FileStore<Header>::Inode::setRight(typename Header::FsSize_t right) {
	this->m_right = right;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::Inode::getRight() {
	return this->m_right;
}

template<typename Header>
void FileStore<Header>::Inode::setData(void *data, typename Header::FsSize_t size) {
	ox_memcpy(this->getData(), data, size);
	m_dataLen = size;
}


template<typename Header>
void *FileStore<Header>::Inode::getData() {
	return this + 1;
}


// FileStore

template<typename Header>
int FileStore<Header>::dumpTo(FileStore<Header> *dest) {
	if (dest->size() >= size()) {
		auto i = ptr<Inode*>(firstInode());
		do {
			dest->write(i->getId(), i->getData(), i->getDataLen(), i->getFileType());
			i = ptr<Inode*>(i->getNext());
		} while (ptr(i) != firstInode());
		return 0;
	} else {
		return -1;
	}
}

template<typename Header>
void FileStore<Header>::resize(typename Header::FsSize_t size) {
	if (size < m_header.getSize()) {
		// shrink file store
		if (m_header.getMemUsed() > size) {
			size = m_header.getMemUsed();
		}
		compact();
		m_header.setSize(size);
	} else if (size > m_header.getSize()) {
		// grow file store
		m_header.setSize(size);
	}
}

template<typename Header>
int FileStore<Header>::write(InodeId_t id, void *data, typename Header::FsSize_t dataLen, uint8_t fileType) {
	auto retval = 1;
	const typename Header::FsSize_t size = sizeof(Inode) + dataLen;
	if (size <= (m_header.getSize() - m_header.getMemUsed())) {
		auto inode = (Inode*) alloc(size);
		if (inode) {
			remove(id);
			inode->setId(id);
			inode->setFileType(fileType);
			inode->setData(data, dataLen);
			auto root = ptr<Inode*>(m_header.getRootInode());
			if (insert(root, inode) || root == inode) {
				retval = 0;
			}
		}
	}
	return retval;
}

template<typename Header>
int FileStore<Header>::remove(InodeId_t id) {
	return remove(ptr<Inode*>(m_header.getRootInode()), id);
}

template<typename Header>
int FileStore<Header>::remove(Inode *root, InodeId_t id) {
	auto err = 1;

	if (root->getId() > id) {
		if (root->getLeft()) {
			auto node = ptr<Inode*>(root->getLeft());
			if (node->getId() != id) {
				err = remove(node, id);
			} else {
				root->setLeft(0);
				if (node->getRight()) {
					insert(root, ptr<Inode*>(node->getRight()));
				}
				if (node->getLeft()) {
					insert(root, ptr<Inode*>(node->getLeft()));
				}
				dealloc(node);
				node->setId(0);
				node->setLeft(0);
				node->setRight(0);
				err = 0;
			}
		}
	} else if (root->getId() < id) {
		if (root->getRight()) {
			auto node = ptr<Inode*>(root->getRight());
			if (node->getId() != id) {
				err = remove(node, id);
			} else {
				root->setRight(0);
				if (node->getRight()) {
					insert(root, ptr<Inode*>(node->getRight()));
				}
				if (node->getLeft()) {
					insert(root, ptr<Inode*>(node->getLeft()));
				}
				dealloc(node);
				node->setId(0);
				node->setLeft(0);
				node->setRight(0);
				err = 0;
			}
		}
	} else if (ptr<Inode*>(m_header.getRootInode())->getId() == id) {
		m_header.setRootInode(root->getRight());
		if (root->getLeft()) {
			insert(ptr<Inode*>(m_header.getRootInode()), ptr<Inode*>(root->getLeft()));
		}
		dealloc(root);
		root->setId(0);
		root->setLeft(0);
		root->setRight(0);
		err = 0;
	}

	return err;
}

template<typename Header>
void FileStore<Header>::dealloc(Inode *inode) {
	auto next = ptr<Inode*>(inode->getNext());
	auto prev = ptr<Inode*>(inode->getPrev());
	prev->setNext(ptr(next));
	next->setPrev(ptr(prev));

	m_header.setMemUsed(m_header.getMemUsed() - inode->size());

	ox_memset(inode, 0, inode->size());
}

template<typename Header>
void FileStore<Header>::updateInodeAddress(InodeId_t id, typename Header::FsSize_t oldAddr, typename Header::FsSize_t newAddr) {
	auto parent = getInodeParent(ptr<Inode*>(m_header.getRootInode()), id, oldAddr);
	if (parent) {
		if (parent->getLeft() == oldAddr) {
			parent->setLeft(newAddr);
		} else if (parent->getRight() == oldAddr) {
			parent->setRight(newAddr);
		}
	}
}

template<typename Header>
int FileStore<Header>::read(InodeId_t id, void *data, typename Header::FsSize_t *size) {
	auto inode = getInode(ptr<Inode*>(m_header.getRootInode()), id);
	int retval = 1;
	if (inode) {
		if (size) {
			*size = inode->getDataLen();
		}
		ox_memcpy(data, inode->getData(), inode->getDataLen());
		retval = 0;
	}
	return retval;
}

template<typename Header>
typename FileStore<Header>::StatInfo FileStore<Header>::stat(InodeId_t id) {
	auto inode = getInode(ptr<Inode*>(m_header.getRootInode()), id);
	StatInfo stat;
	if (inode) {
		stat.size = inode->getDataLen();
		stat.fileType = inode->getFileType();
		stat.inodeId = id;
	} else {
		stat.inodeId = 0;
	}
	return stat;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::spaceNeeded(InodeId_t id, typename Header::FsSize_t size) {
	typename Header::FsSize_t needed = sizeof(Inode) + size;;
	auto inode = getInode(ptr<Inode*>(m_header.getRootInode()), id);
	if (inode) {
		needed -= inode->size();
	}
	return needed;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::size() {
	return m_header.getSize();
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::available() {
	return m_header.getSize() - m_header.getMemUsed();
}

template<typename Header>
typename FileStore<Header>::Inode *FileStore<Header>::getInode(Inode *root, InodeId_t id) {
	Inode *retval = nullptr;

	if (root->getId() > id) {
		if (root->getLeft()) {
			retval = getInode(ptr<Inode*>(root->getLeft()), id);
		}
	} else if (root->getId() < id) {
		if (root->getRight()) {
			retval = getInode(ptr<Inode*>(root->getRight()), id);
		}
	} else if (root->getId() == id) {
		retval = root;
	}

	return retval;
}

template<typename Header>
typename FileStore<Header>::Inode *FileStore<Header>::getInodeParent(Inode *root, InodeId_t id, typename Header::FsSize_t targetAddr) {
	Inode *retval = nullptr;

	if (root->getId() > id) {
		if (root->getLeft()) {
			if (root->getLeft() == targetAddr) {
				retval = root;
			} else {
				retval = getInodeParent(ptr<Inode*>(root->getLeft()), id, targetAddr);
			}
		}
	} else if (root->getId() < id) {
		if (root->getRight()) {
			if (root->getRight() == targetAddr) {
				retval = root;
			} else {
				retval = getInodeParent(ptr<Inode*>(root->getRight()), id, targetAddr);
			}
		}
	}

	return retval;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::nextInodeAddr() {
	typename Header::FsSize_t next = ptr(lastInode()) + lastInode()->size();
	return next;
}

template<typename Header>
void *FileStore<Header>::alloc(typename Header::FsSize_t size) {
	typename Header::FsSize_t next = nextInodeAddr();
	if ((next + size) > (uint64_t) end()) {
		compact();
		next = nextInodeAddr();
		if ((next + size) > (uint64_t) end()) {
			return nullptr;
		}
	}

	const auto retval = next;
	const auto inode = ptr<Inode*>(retval);
	ox_memset(inode, 0, size);
	inode->setPrev(ptr<Inode*>(firstInode())->getPrev());
	inode->setNext(retval + size);
	m_header.setMemUsed(m_header.getMemUsed() + size);
	ptr<Inode*>(firstInode())->setPrev(retval);
	return inode;
}

template<typename Header>
void FileStore<Header>::compact() {
	auto dest = ptr<Inode*>(firstInode());
	auto current = ptr<Inode*>(firstInode());
	while (current->getNext() > firstInode() && current->getNext() < ptr(end())) {
		ox_memcpy(dest, current, current->size());
		if (dest->getNext() != firstInode()) {
			dest->setNext(ptr(dest) + dest->size());
		}
		ptr<Inode*>(dest->getNext())->setPrev(ptr(dest));
		updateInodeAddress(dest->getId(), ptr(current), ptr(dest));
		current = ptr<Inode*>(dest->getNext());
		dest = ptr<Inode*>(ptr(dest) + dest->size());
	}
}

template<typename Header>
bool FileStore<Header>::insert(Inode *root, Inode *insertValue) {
	auto retval = false;

	if (root->getId() > insertValue->getId()) {
		if (root->getLeft()) {
			retval = insert(ptr<Inode*>(root->getLeft()), insertValue);
		} else {
			root->setLeft(ptr(insertValue));
			retval = true;
		}
	} else if (root->getId() < insertValue->getId()) {
		if (root->getRight()) {
			retval = insert(ptr<Inode*>(root->getRight()), insertValue);
		} else {
			root->setRight(ptr(insertValue));
			retval = true;
		}
	} else if (m_header.getRootInode() == 0) {
		m_header.setRootInode(ptr(insertValue));
		retval = true;
	}

	return retval;
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::iterator() {
	return ptr(lastInode()) + lastInode()->size();
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::ptr(void *ptr) {
#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif
	return ((uint8_t*) ptr) - begin();
#ifdef _MSC_VER
#pragma warning(default:4244)
#endif
}

template<typename Header>
typename Header::FsSize_t FileStore<Header>::firstInode() {
	return sizeof(FileStore<Header>);
}

template<typename Header>
typename FileStore<Header>::Inode *FileStore<Header>::lastInode() {
	return ptr<Inode*>(ptr<Inode*>(firstInode())->getPrev());
}

template<typename Header>
uint16_t FileStore<Header>::fsType() {
	return m_header.getFsType();
};

template<typename Header>
uint16_t FileStore<Header>::version() {
	return m_header.getVersion();
};

template<typename Header>
uint8_t *FileStore<Header>::format(uint8_t *buffer, typename Header::FsSize_t size, uint16_t fsType) {
	ox_memset(buffer, 0, size);

	auto *fs = (FileStore*) buffer;
	fs->m_header.setFsType(fsType);
	fs->m_header.setVersion(Header::VERSION);
	fs->m_header.setSize(size);
	fs->m_header.setMemUsed(sizeof(FileStore<Header>) + sizeof(Inode));
	fs->m_header.setRootInode(sizeof(FileStore<Header>));
	((Inode*) (fs + 1))->setPrev(sizeof(FileStore<Header>));
	fs->lastInode()->setNext(sizeof(FileStore<Header>));

	return (uint8_t*) buffer;
}

typedef FileStore<FileStoreHeader<uint16_t, uint16_t>> FileStore16;
typedef FileStore<FileStoreHeader<uint32_t, uint16_t>> FileStore32;
typedef FileStore<FileStoreHeader<uint64_t, uint16_t>> FileStore64;

}
}
