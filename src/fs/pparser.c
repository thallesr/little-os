#include "pparser.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/memory.h"
#include "status.h"
#include "config.h"
#include "memory/heap/kheap.h"

static int pathparser_path_valid_format(const char* filename)
{
    int len = strnlen(filename,PEACH_OS_MAX_PATH);
    return (len >=3 && is_digit(filename[0]) && memcmp((void*) &filename[1],":/",2)==0);
}
//should it be const in this case? since it modifies
static int pathparser_path_get_drive_by_path_and_consume(const char ** path){
    if (! pathparser_path_valid_format(* path)){
        return -EBADPATH;
    }

    int drive_no = to_numeric_digit(*path[0]);

    *path += 3;
    return drive_no;

}

static struct path_root * pathparser_create_root(int drive_number){
    struct path_root* pathRoot = kzalloc(sizeof(struct path_root));
    pathRoot -> drive_no = drive_number;
    pathRoot -> first = 0;
    return pathRoot;
}

static const char * pathparser_get_path_part_and_consumes(const char ** path)
{
    char * result_path_part = kzalloc(PEACH_OS_MAX_PATH);
    int i =0;
    while (** path != '/' && **path != 0x00){
       result_path_part[i] = **path;
       *path +=1;
       i++;
    }

    if (**path =='/'){
        * path +=1;
        //skip /
    }

    if (i==0){
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;

}

struct path_part * pathparser_parse_path_part(struct path_part * last_part,
                                             const char ** path){
    const char * path_part_str = pathparser_get_path_part_and_consumes(path);

    if (!path_part_str){
        return 0;
    }

    struct path_part * part = kzalloc(sizeof(struct path_part));
    part-> part = path_part_str;
    part-> next =0x00;

    if (last_part){
        last_part-> next = part;
    }

    return part;
}
void pathparser_free( struct path_root * root){
    struct path_part* part = root ->first;
    while(part ){
        struct path_part * next_part = part ->next;
        kfree((void*) part-> part);
        kfree(part);
        part = next_part;
    }

    kfree(root);

}
struct path_root * pathparser_parse(const char* path, const char* current_director_path){
    int res =0;
    const char * tmp_path=path;

    struct path_root * path_root= 0;

    if (strlen(path) > PEACH_OS_MAX_PATH){
        goto out;
    }

    res = pathparser_path_get_drive_by_path_and_consume(&tmp_path);
    if (res <0){
        goto out;
    }

    path_root = pathparser_create_root(res);
    if (!path_root){
        goto out;
    }

    struct path_part * first_part = pathparser_parse_path_part(NULL, &tmp_path);
    if (!first_part){
        goto out;
    }

    path_root -> first = first_part;
    struct path_part *part = pathparser_parse_path_part( first_part, &tmp_path);
    while (part)
    {
        part = pathparser_parse_path_part(part, &tmp_path);
    }

    out:
        return path_root;
}
