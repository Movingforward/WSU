import urllib.request
import webbrowser


def format_search_term(search_term):
    '''
    To prepare the search term string for the query:
    1. remove comma
    2. replace spaces with +
    '''
    search_term = search_term.replace(",", "")
    search_term = search_term.replace(" ", "+")
    return search_term


def build_query(query):
    '''
    Builds the query string for the Spotify Search API according to this website:
    https://developer.spotify.com/web-api/console/get-search-item/
    '''
    query_base = "https://api.spotify.com/v1/search?q="
    query = query_base + query
    # perform a track search and only return the top result
    query += "&type=track&limit=1"
    return query


def extract_numeric_value(results_str, label):
    '''
    Extracts an integer value represented by the label parameter from the JSON response.
    '''
    index = results_str.find(label)
    results_str = results_str[index:]
    index = results_str.find(":")
    results_str = results_str[index + 2:]
    index = results_str.find(",")
    results_str = results_str[:index]
    value = int(results_str)
    return value


def extract_preview_url(results_str):
    '''
    Extracts the preview url from the JSON response.
    '''
    index = results_str.find("preview_url")
    results_str = results_str[index:]
    index = results_str.find(":")
    results_str = results_str[index:]
    index = results_str.find("\"")
    results_str = results_str[index + 1:]
    index = results_str.find("\"")
    results_str = results_str[:index]
    return results_str


def get_track_information(track, artist, info_type):
    '''
    Accepts 3 strings representing a track and its artist, and the name of the information to retrieve (info_type).
    info_type can be:
        "duration_ms": returns an integer representing the duration of the song in MILLISECONDS
        "popularity": returns an integer representing the popularity of the song
        "preview_url": returns a string representing a url hosting a short clip of the song
    Returns the requested information for track and artist
    '''
    track = format_search_term(track)
    artist = format_search_term(artist)

    # search the spotify database for a track by artist
    search_terms = track + "+" + artist

    query = build_query(search_terms)

    web_obj = urllib.request.urlopen(query)
    # web_obj.read() returns an array of bytes, need to convert to a string
    results_str = str(web_obj.read())
    web_obj.close()

    info = ""
    if info_type == "popularity" or info_type == "duration_ms":
        info = extract_numeric_value(results_str, info_type)
    elif info_type == "preview_url":
        info = extract_preview_url(results_str)
    else:
        print("Unknown information type")
    return info

def main():

    infile = open("top10alltime.txt", "r")
    outfile = open("top10alltime_stats.txt", "w")
    count = 0
    totalSongLenths = 0
    totalPopularity = 0
    longestsongname = None
    longestsongartist = None
    longestsonglength = 0
    mostpopsongname = None
    mostpopsongrating = 0
    mostpopsongartist = None
    print("Reading in playlist information from %s" % (infile.name))
    outfile.write("The results from Spotify\n\n")
    while True:

        track = infile.readline()
        if track == "":
            break
        # remove newline \n from end
        track = track.strip()
        artist = infile.readline()
        # remove newline \n from end
        artist = artist.strip()
        # read in the new line that delimits track/artist pairs
        infile.readline()
        print("Querying Spotify for information regarding")
        print(track, artist, sep=" by ")

        duration = get_track_information(track,artist,'duration_ms')
        secduration = duration / 1000 #Time in seconds
        minduration = int(secduration / 60) #Time in minutes
        remsecduration = int(secduration % 60)
        popularity = get_track_information(track,artist,'popularity')
        s = "\nThe duration of the song is: "
        r = "\nThe rating of the song is: "
        min = " minutes and "
        sec = " seconds"

        outfile.write("\n")
        outfile.writelines(track)
        outfile.write(" by ")
        outfile.writelines(artist)
        outfile.write(s)
        outfile.write(str(minduration))
        outfile.write(min)
        outfile.write(str(remsecduration))
        outfile.write(sec)
        outfile.write(r)
        outfile.write(str(popularity))

        if mostpopsongrating < popularity:
            mostpopsongrating = popularity
            mostpopsongname = track
            mostpopsongartist = artist


        if longestsonglength < duration:
            longestsonglength = duration
            longestsongname = track
            longestsongartist = artist


        totalPopularity += popularity
        totalSongLenths += duration
        count = count + 1

    print("Writing Playlist Stats to %s" % (outfile.name))
    outfile.write("\nThe Total Number of Songs: ")
    outfile.write(str(count))
    outfile.write("\nThe average song length is: ")
    averagesec = (totalSongLenths / count) / 1000
    averagemin = int(averagesec / 60)
    averageremsec = int(averagesec % 60)
    outfile.write(str(averagemin))
    outfile.write(" minutes, and ")
    outfile.write(str(averageremsec))
    outfile.write(" seconds")
    outfile.write("\nThe average song popularity is: ")
    averagepop = totalPopularity / count
    outfile.write(str(averagepop))
    outfile.write("\nThe longest song is: ")
    outfile.write(longestsongname)
    outfile.write(" by ")
    outfile.write(str(longestsongartist))
    outfile.write(" whose length is:")
    seclongestsonglen= longestsonglength / 1000
    minlongestsonglen = int(seclongestsonglen / 60)
    remseclongestsonglen = int(seclongestsonglen % 60)
    outfile.write(str(minlongestsonglen))
    outfile.write(" minutes, and ")
    outfile.write(str(remseclongestsonglen))
    outfile.write(" seconds")
    outfile.write("\nThe most popular song is: ")
    outfile.write(str(mostpopsongname))
    outfile.write(" by ")
    outfile.write(str(mostpopsongartist))
    outfile.write(" which was rated at: ")
    outfile.write(str(mostpopsongrating))

    url = get_track_information(mostpopsongname,mostpopsongartist,"preview_url")
    webbrowser.open(url)

    print("\nClosing Files\n")

    infile.close()
    outfile.close()

main()
