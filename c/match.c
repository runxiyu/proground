int match(const char *mask, const char *name)
{
	const char *m = mask, *n = name;
	const char *m_tmp = mask, *n_tmp = name;
	int star_p;

	s_assert(mask != NULL);
	s_assert(name != NULL);

	for (;;) {
		switch (*m) {
		case '\0':
			if (!*n)
				return 1;
 backtrack:
			if (m_tmp == mask)
				return 0;
			m = m_tmp;
			n = ++n_tmp;
			break;
		case '*':
		case '?':
			for (star_p = 0;; m++) {
				if (*m == '*')
					star_p = 1;
				else if (*m == '?') {
					if (!*n++)
						goto backtrack;
				} else
					break;
			}
			if (star_p) {
				if (!*m)
					return 1;
				else {
					m_tmp = m;
					for (n_tmp = n;
					     *n
					     && irctolower(*n) !=
					     irctolower(*m); n++) ;
				}
			}
			/* and fall through */
		default:
			if (!*n)
				return (*m != '\0' ? 0 : 1);
			if (irctolower(*m) != irctolower(*n))
				goto backtrack;
			m++;
			n++;
			break;
		}
	}
}
